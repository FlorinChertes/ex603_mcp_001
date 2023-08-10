#include <syncstream>
#include <iostream>

#include <list>
#include <utility>     // for std::exchange()
#include <functional>  // for std::function
#include <coroutine>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

#include <cassert>

class CoroPool;

class [[nodiscard]] CoroPoolTask
{
    inline static int maxId = 0;
    inline static int finalAwaiterMaxId = 0;
    int id;

  friend class CoroPool;

public:
  struct promise_type;
  using CoroHdl = std::coroutine_handle<promise_type>;

private:
  CoroHdl hdl;

public:
  struct promise_type {
    CoroPool* poolPtr = nullptr;   // if not null, lifetime is controlled by pool
    CoroHdl contHdl = nullptr;     // coro that awaits this coro
  
    CoroPoolTask get_return_object() noexcept {
      return CoroPoolTask{CoroHdl::from_promise(*this)};
    }
    auto initial_suspend() const noexcept { return std::suspend_always{}; }
    void unhandled_exception() noexcept { std::exit(1); }
    void return_void() noexcept { }

    auto final_suspend() const noexcept {

      struct FinalAwaiter {
          int id;

          FinalAwaiter()
              : id{++finalAwaiterMaxId} {
              std::osyncstream(std::cout) << "          FinalAwaiter"
                  << " id: " << id << ": ==> constructor" << std::endl;
          }
          ~FinalAwaiter() {
              finalAwaiterMaxId--;
              std::osyncstream(std::cout) << "          FinalAwaiter"
                  << " id: " << id << ": <== destructor" << std::endl;
          }

        bool await_ready() const noexcept { return false; }

        std::coroutine_handle<> await_suspend(CoroHdl h) noexcept {
          std::osyncstream(std::cout)
          << "hdl: " << h.address()
          << " id: " << id
          << " FinalAwaiter::await_suspend on thread: " << std::this_thread::get_id()
          << std::endl;

          if (h.promise().contHdl) {
                std::osyncstream(std::cout)
                << "from promise cont hdl: " << h.promise().contHdl.address()
                << std::endl;
            return h.promise().contHdl;    // resume continuation
          }
          else {
            return std::noop_coroutine();  // no continuation
          }

        }

        void await_resume() noexcept { }
      };

      return FinalAwaiter{};  // AFTER suspended, resume continuation if there is one
    }
  };

  explicit CoroPoolTask(CoroHdl handle)
      : id{++maxId}, hdl { handle } {
      std::osyncstream(std::cout) << "        INTERFACE: constructor hdl: "
          << hdl.address() << " id: " << id << std::endl;
  }
  ~CoroPoolTask() {
      maxId--;
    if (hdl && !hdl.promise().poolPtr) {
      // task was not passed to pool:
      hdl.destroy();
    }
    std::osyncstream(std::cout) << "        INTERFACE: destructor hdl: "
        << hdl.address() << " id: " << id << std::endl;
  }

  CoroPoolTask(const CoroPoolTask&) = delete;
  CoroPoolTask& operator= (const CoroPoolTask&) = delete;

  CoroPoolTask(CoroPoolTask&& t)
      : id{t.id}, hdl {t.hdl} {
      t.id = 0;
      t.hdl = nullptr;
  }

  CoroPoolTask& operator= (CoroPoolTask&&) = delete;

  // Awaiter for: co_await task()
  // - queues the new coro in the pool
  // - sets the calling coro as continuation
  struct CoAwaitAwaiter {

      inline static int maxId = 0;
      int id;

      CoroHdl newHdl;

    CoAwaitAwaiter(CoroHdl newHdl_)
        : id{++maxId}, newHdl { newHdl_} {
        std::osyncstream(std::cout) << "          CoAwaitAwaiter, new hdl:"
            << newHdl.address() << " id: " << id << ": ==> constructor\n";
    }
    ~CoAwaitAwaiter() {
        maxId--;
        std::osyncstream(std::cout) << "          CoAwaitAwaiter, new hdl:"
            << newHdl.address() << " id: " << id << ": <== destructor\n";
    }

    bool await_ready() const noexcept { return false; }
    void await_suspend(CoroHdl awaitingHdl) noexcept;  // see below
    void await_resume() noexcept {}
  };

  auto operator co_await() noexcept {
        std::osyncstream(std::cout) << " CoroPoolTask::operator co_await() : CoAwaitAwaiter"
        << " hdl: " << hdl.address() << std::endl;
    return CoAwaitAwaiter{std::exchange(hdl, nullptr)}; // pool takes ownership of hdl
  }
};

class CoroPool
{

private:
  std::list<std::jthread> threads;         // list of threads
  std::list<CoroPoolTask::CoroHdl> coros;  // queue of scheduled coros

  std::mutex corosMx;
  std::condition_variable_any corosCV;

  std::atomic<int> numCoros = 0;           // counter for all coros owned by the pool

public:

  explicit CoroPool(int num) {
    // start pool with num threads:
    for (int i = 0; i < num; ++i) {
      std::jthread worker_thread{[this](std::stop_token st) {
                                   threadLoop(st);
                                 }};
      threads.push_back(std::move(worker_thread));
    }
  }

  ~CoroPool() {
    for (auto& t : threads) {  // request stop for all threads
      t.request_stop();
    }
    for (auto& t : threads) {  // wait for end of all threads
      t.join();
    }
    for (auto& c : coros) {    // destroy remaining coros
      c.destroy();
    }
  }

  CoroPool(CoroPool&) = delete;
  CoroPool& operator=(CoroPool&) = delete;

  void runTask(CoroPoolTask&& coroTask) noexcept {
      std::osyncstream(std::cout) << "runTask"
          << " on thread: " << std::this_thread::get_id()
          << " coroTask.hdl: " << coroTask.hdl.address() << std::endl;
      auto hdl = std::exchange(coroTask.hdl, nullptr); // pool takes ownership of hdl

    assert(hdl != nullptr);
    if (hdl.done()) {
        hdl.destroy();  // OOPS, a done() coroutine was passed
    }
    else {
      // schedule coroutine in the pool
        runCoro(hdl);
    }
  }

  // runCoro(): let pool run (and control lifetime of) coroutine
  // called from:
  // - pool.runTask(CoroPoolTask)
  // - co_await task()
  void runCoro(CoroPoolTask::CoroHdl coroHdl) noexcept {
    std::osyncstream(std::cout) << "runCoro start" << " numCoro: "
        << numCoros << " on thread: " << std::this_thread::get_id() << std::endl;

    ++numCoros;
    coroHdl.promise().poolPtr = this;  // disables destroy in CoroPoolTask
    {
      std::scoped_lock lock(corosMx);
      coros.push_front(coroHdl);       // queue coro
      corosCV.notify_one();         // and let one thread resume it
    }

    std::osyncstream(std::cout) << "runCoro end" << " numCoro: " << numCoros << " on thread: " << std::this_thread::get_id() << std::endl;
  }

  void threadLoop(std::stop_token st) {
    int i{0};

    while (!st.stop_requested()) {

      // get next coro task from the queue:
      std::osyncstream(std::cout) << "\nBEGINN threadLoop: " << i << " on thread: " << std::this_thread::get_id() << std::endl;
      CoroPoolTask::CoroHdl coro;
      {
        std::unique_lock lock(corosMx);
        if (!corosCV.wait(lock, st, [&] {
                                      return !coros.empty();
                                    })) {
            std::osyncstream(std::cout) << "before return threadLoop: " << i << " on thread: " << std::this_thread::get_id() << std::endl;
            return;  // stop requested
        }
        coro = coros.back();
        coros.pop_back();
      }

      // resume it:
      std::osyncstream(std::cout) << "\nbefore resume threadLoop: " << i << " on thread: " << std::this_thread::get_id() << std::endl;
      coro.resume();  // RESUME
      std::osyncstream(std::cout) << "after resume threadLoop: " << i << " on thread: " << std::this_thread::get_id() << std::endl;

      // NOTE: The coro initially resumed on this thread might NOT be the coro finally called.
      // If a main coro awaits a sub coro, then the thread that finally resumed the sub coro
      // resumes the main coro as its continuation.
      // => After this resumption, this coro and SOME continuations MIGHT be done
      std::function<void(CoroPoolTask::CoroHdl)> destroyDone;
      destroyDone = [&destroyDone, this, i](auto hdl) {
                      if (hdl && hdl.done()) {
                        auto nextHdl = hdl.promise().contHdl;

                        std::osyncstream(std::cout) << "destroy hdl: " << hdl.address()
                            << " cont hdl: " << nextHdl.address() << std::endl;
                        hdl.destroy();         // destroy handle done
                        --numCoros;            // adjust total number of coros

                        std::osyncstream(std::cout) << "destroy recursive threadLoop: "  << i << " / " << numCoros
                            << " on thread: " << std::this_thread::get_id()
                            << std::endl;

                        destroyDone(nextHdl);  // do it for all continuations done
                      }
                    };

      std::osyncstream(std::cout) << "\nbefore destroy threadLoop: " << i << " / " << numCoros
          << " on thread: " << std::this_thread::get_id() << std::endl;

      destroyDone(coro);      // recursively destroy coroutines done

      std::osyncstream(std::cout) << "after destroy threadLoop: " << i << " / " << numCoros
          << " on thread: " << std::this_thread::get_id() << std::endl;


      numCoros.notify_all();  // wake up any waiting waitUntilNoCoros()

      // sleep a little to force another thread to be used next:
      std::osyncstream(std::cout) << "END SLEEP threadLoop: " << i
          << " on thread: " << std::this_thread::get_id() << '\n' << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds{100});
      std::osyncstream(std::cout) << "END threadLoop: " << i
          << " on thread: " << std::this_thread::get_id() << '\n' << std::endl;
      ++i;
    }
  }

  void waitUntilNoCoros() {
    int num = numCoros.load();
    std::osyncstream(std::cout) << " numCoros = "
        << num << " on thread: " << std::this_thread::get_id() << std::endl;
    while (num > 0) {
      numCoros.wait(num);  // wait for notification that numCoros changed the value
      num = numCoros.load();
      std::osyncstream(std::cout) << " numCoros = "
          << num << " on thread: " << std::this_thread::get_id() << std::endl;
    }
  }
};

// CoroPoolTask awaiter for: co_await task()
// - sets the calling coro as continuation
// - queues the new coro in the pool
void CoroPoolTask::CoAwaitAwaiter::await_suspend(CoroHdl awaitingHdl) noexcept
{
  std::osyncstream(std::cout) << "CoAwaitAwaiter await_suspend"
      << " new hdl: " << newHdl.address()
      << " awaiting hdl: " << awaitingHdl.address() << std::endl;

  newHdl.promise().contHdl = awaitingHdl;
  awaitingHdl.promise().poolPtr->runCoro(newHdl);
}

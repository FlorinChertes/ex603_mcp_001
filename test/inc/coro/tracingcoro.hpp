#include <iostream>
#include <coroutine>
#include <exception>  // for terminate()

// coroutine interface to deal with a simple task
// - providing resume() to resume it
class [[nodiscard]] TracingCoro {

public:
  // native coroutine handle and its promise type:
  struct promise_type;

private:
  using CoroHdl = std::coroutine_handle<promise_type>;

  CoroHdl hdl;          // coroutine handle

public:
  // helper type for state and customization:
  struct promise_type {
    promise_type() {
      std::cout << "      PROMISE: constructor\n";
    }
    ~promise_type() {
      std::cout << "      PROMISE: destructor\n";
    }
    auto get_return_object() {       // init and return the coroutine interface
      std::cout << "      PROMISE: get_return_object()\n";
      return TracingCoro{CoroHdl::from_promise(*this)};
    }
    auto initial_suspend() {         // initial suspend point
      std::cout << "      PROMISE: initial_suspend()\n";
      // use return std::suspend_never{};  to start eagerly
      return std::suspend_always{};  // - start lazily
    }
    void unhandled_exception() {     // deal with exceptions
      std::cout << "      PROMISE: unhandled_exception()\n";
      std::terminate();              // - terminate the program
    }
    void return_void() {             // deal with the end or co_return;
      std::cout << "      PROMISE: return_void()\n";
    }
    auto final_suspend() noexcept {  // final suspend point
      std::cout << "      PROMISE: final_suspend()\n";
      return std::suspend_always{};  // - suspend immediately
    }
  };

  // constructor and destructor:
  TracingCoro(auto h)
   : hdl{h} {           // store coroutine handle in interface
      std::cout << "        INTERFACE: constructor\n";
  }
  ~TracingCoro() {
    std::cout << "        INTERFACE: destructor start\n";
    if (hdl) {
      hdl.destroy();    // destroy coroutine handle
    }
    std::cout << "        INTERFACE: destructor end\n";
  }
  // don't copy or move:
  TracingCoro(const TracingCoro&) = delete;
  TracingCoro& operator=(const TracingCoro&) = delete;

  // API to resume the coroutine
  // - returns whether there is still something to process
  bool resume() const {
    std::cout << "        INTERFACE: resume()\n";
    if (!hdl || hdl.done()) {
      return false;                     // nothing (more) to process
    }
    hdl.resume();                       // RESUME
    return !hdl.done();
  }
};

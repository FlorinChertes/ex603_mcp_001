// Rainer Grimm

#include <coroutine>
#include <chrono>
#include <iostream>
#include <functional>
#include <string>
#include <stdexcept>
#include <atomic>
#include <thread>
#include <cassert>

class Event {
 public:

    Event() = default;

    Event(const Event&) = delete;
    Event(Event&&) = delete;
    Event& operator=(const Event&) = delete;
    Event& operator=(Event&&) = delete;

    class Awaiter;

    Awaiter operator co_await() const noexcept;
    void notify() noexcept;

    int get_value() const {
        return value_;
    }

 private:

    friend class Awaiter;

    mutable std::atomic<void*> suspendedWaiter{nullptr};
    mutable std::atomic<bool> notified{false};
    mutable std::atomic<int> value_{0};

};

class Event::Awaiter {
 public:
  Awaiter(const Event& eve): event(eve) {}

  bool await_ready() const;
  bool await_suspend(std::coroutine_handle<> corHandle) noexcept;
  void await_resume() noexcept {

      std::cout << "Receiver received ready (await_resume) value = "
          << event.value_ << std::endl;

      event.value_ = 0;
  }

 private:
    friend class Event;

    const Event& event;
    std::coroutine_handle<> coroutineHandle;
};

bool Event::Awaiter::await_ready() const
{
    std::cout << "in await_ready event.notified = "
        << event.notified << std::endl;

    // allow at most one waiter
    if (event.suspendedWaiter.load() != nullptr){
        throw std::runtime_error("More than one waiter is not valid");
    }

    // event.notified == false; suspends the coroutine
    // event.notified == true; the coroutine is executed such as a usual function
     return event.notified;
}
                                                                     // (8)
bool Event::Awaiter::await_suspend(std::coroutine_handle<> corHandle) noexcept
{
    std::cout << "in await_suspend event.notified = "
        << event.notified << std::endl;

    coroutineHandle = corHandle;
    if (event.notified)
    {
        std::cout << "Receiver received ready (await_suspend)." << std::endl;
        return false; // resumes the current coroutine
    }

    // store the waiter for later notification
    event.suspendedWaiter.store(this);

    return true; // returns control to the caller/resumer
}

void Event::notify() noexcept
{
    std::cout << "Sender send ready." << std::endl;
    value_ = 5;

    // try to load the waiter
    auto* waiter = static_cast<Awaiter*>(suspendedWaiter.load());

    // check if a waiter is available
    if (waiter != nullptr) {
        // resume the coroutine => await_resume
        assert(waiter->coroutineHandle);
        waiter->coroutineHandle.resume();
    }
    else
    {
        notified = true;
    }

}

Event::Awaiter Event::operator co_await() const noexcept {
    return Awaiter{ *this };
}

struct Task {
    struct promise_type {
        Task get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
};

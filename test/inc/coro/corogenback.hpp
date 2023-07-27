#include "backawaiter.hpp"
#include <coroutine>
#include <exception>   // for terminate()
#include <string>
#include <iostream>

class [[nodiscard]] CoroGenBack {
public:
  struct promise_type;
  using CoroHdl = std::coroutine_handle<promise_type>;
private:
  CoroHdl hdl;                         // native coroutine handle

public:
  struct promise_type {
    int coroValue = 0;                 // value TO caller on suspension
    std::string backValue;             // value back FROM caller after suspension

    auto yield_value(int val) {        // reaction to co_yield
      std::cout << "      PROMISE: yield_value\n";
      coroValue = val;                 // - store value locally
      backValue.clear();               // - reinit back value
      return BackAwaiter<CoroHdl>{};   // - use special awaiter for response
    }

    // the usual members:
    auto get_return_object() {
        std::cout << "      PROMISE: get_return_object()\n";
        return CoroHdl::from_promise(*this); }
    auto initial_suspend() {
        std::cout << "      PROMISE: initial_suspend()\n";
        return std::suspend_always{}; }
    void return_void() {
        std::cout << "      PROMISE: return_void()\n";
    }
    void unhandled_exception() { std::terminate(); }
    auto final_suspend() noexcept {
        std::cout << "      PROMISE: final_suspend()\n";
        return std::suspend_always{}; }
  };

  // constructors and destructor:
  CoroGenBack(auto h) : hdl{h} { }
  ~CoroGenBack() { if (hdl) hdl.destroy(); }

  // no copying or moving:
  CoroGenBack(const CoroGenBack&) = delete;
  CoroGenBack& operator=(const CoroGenBack&) = delete;

  // API:
  // - resume the coroutine:
  bool resume() const {
    if (!hdl || hdl.done()) {
      return false;    // nothing (more) to process
    }
    hdl.resume();      // RESUME
    return !hdl.done();
  }

  // - yield value from co_yield:
  int getValue() const {
    std::cout << "      PROMISE: getValue\n";
    return hdl.promise().coroValue;
  }

  // - set value back to the coroutine after suspension:
  void setBackValue(const auto& val) {
    std::cout << "      PROMISE: setBackValue\n";
    hdl.promise().backValue = val;
  }
};

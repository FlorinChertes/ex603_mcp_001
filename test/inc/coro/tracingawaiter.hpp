#include <iostream>

class TracingAwaiter {
  inline static int maxId = 0;
  int id;
public:
  TracingAwaiter() : id{++maxId} {
    std::cout << "          AWAITER" << id << ": ==> constructor\n";
  }
  ~TracingAwaiter() {
    std::cout << "          AWAITER" << id << ": <== destructor\n";
  }
  // don't copy or move:
  TracingAwaiter(const TracingAwaiter&) = delete;
  TracingAwaiter& operator=(const TracingAwaiter&) = delete;

  //constexpr
  bool await_ready() const noexcept {
    std::cout << "          AWAITER" << id << ":     await_ready()\n";
    return false; // true: do NOT (try to) suspend
  }

  // Return type/value means:
  // - void: do suspend
  // - bool: true: do suspend
  // - handle: resume coro of the handle
  //constexpr
  bool await_suspend(auto) const noexcept {
    std::cout << "          AWAITER" << id << ":     await_suspend()\n";
    return true;
  }

  //constexpr
  void await_resume() const noexcept {
    std::cout << "          AWAITER" << id << ":     await_resume()\n";
  }
};

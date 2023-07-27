#include <iostream>

template<typename Hdl>
class BackAwaiter {
  inline static int maxId = 0;
  int id;
  Hdl hdl = nullptr;   // coroutine handle saved from await_suspend() for await_resume()
public:
  BackAwaiter() : id{ ++maxId } {
      std::cout << "\n          AWAITER" << id << ": ==> constructor\n";
  }
  ~BackAwaiter() {
      std::cout << "          AWAITER" << id << ": <== destructor\n\n";
  }

  // don't copy or move:
  BackAwaiter(const BackAwaiter&) = delete;
  BackAwaiter& operator=(const BackAwaiter&) = delete;

  bool await_ready() const noexcept {
    std::cout << "          AWAITER" << id << ":     await_ready()\n";
    return false;                    // do suspend
  }

  void await_suspend(Hdl h) noexcept {
    std::cout << "          AWAITER" << id << ":     await_suspend()\n";
    hdl = h;                         // save handle to get access to its promise
  }

  auto await_resume() const noexcept {
    std::cout << "          AWAITER" << id << ":     await_resume()\n";
    return hdl.promise().backValue;  // return back value stored in the promise
  }
};

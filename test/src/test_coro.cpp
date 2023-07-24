#include "../inc/coro/tracingcoro.hpp"
#include "../inc/coro/tracingawaiter.hpp"

#include <iostream>

TracingCoro coro(int max)
{
  std::cout << "  START coro(" << max << ")\n";

  for (int i = 1; i <= max; ++i)
  {
    std::cout << "  SUSPEND coro(" << i << '/' << max << ") suspend in coro\n";
    co_await TracingAwaiter{};          // SUSPEND
    std::cout << "  CONTINUE coro(" << max << ")\n";
  }

  std::cout << "  END coro(" << max << ")\n";
}

void test_065()
{
    std::cout << "*** test 065 ***" << std::endl;
    std::cout << '\n';

    // start coroutine:
    std::cout << "**** start coro()\n";
    auto coroTask = coro(3);              // init coroutine
    std::cout << "**** coro() started\n";

    // loop to resume the coroutine until it is done:
    std::cout << "\n**** coro caller, before first resume of coro() in loop\n";
    while (coroTask.resume())
    {           // RESUME
        std::cout << "**** coro caller, coro() is suspended in loop\n";
        //...
        std::cout << "\n**** coro caller, coro() before resumes in loop\n";
    }

    std::cout << "\n**** coro() loop done\n";
}

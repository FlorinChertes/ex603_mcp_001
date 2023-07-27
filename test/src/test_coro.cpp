#include "../inc/coro/corogenback.hpp"   // for CoroGenBack

#include "../inc/coro/tracingcoro.hpp"
#include "../inc/coro/tracingawaiter.hpp"

#include <iostream>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
CoroGenBack coro_gen(int max)
{
    std::cout << "         CORO " << max << " start\n";

    for (int val = 1; val <= max; ++val) {
        // print next value:
        std::cout << "         CORO suspend and send val: " << val << '/' << max << '\n';

        // yield next value:
        auto back = co_yield val;           // SUSPEND with value and response
        std::cout << "         CORO resume and receive val: " << back << "\n";
    }

    std::cout << "         CORO " << max << " end\n";
}


void test_065_01()
{
    std::cout << "\n*** test 065_01 ***" << std::endl;
    std::cout << '\n';

    // start coroutine:
    auto coroGen = coro_gen(3);              // initialize coroutine
    std::cout << "**** coro() started\n";

    // loop to resume the coroutine until it is done:
    std::cout << "\n**** coro() first resume\n";
    while (coroGen.resume()) {           // RESUME
        // process value from co_yield:
        auto val = coroGen.getValue();
        std::cout << "**** coro caller, coro() is suspended, get value from coro: " << val << '\n';

        // set response (the value co_yield yields):
        std::string back = (val % 2 != 0 ? "OK" : "ERR");
        std::cout << "\n**** coro caller, coro() before resumes, set back value to coro: " << back << '\n';
        coroGen.setBackValue(back);        // set value back to the coroutine
    }

    std::cout << "**** coro() ended\n";
}

#include "coropriosched.hpp"
#include <iostream>

CoroPrioTask coro_prio(int max)
{
  std::cout << "    start coro(" << max << ")\n";
  for (int val = 1; val <= max; ++val) {
    std::cout << "    coro(" << max << "): " << val << '\n';
    co_await CoroPrio{CoroPrioRequest::less};   // SUSPEND with lower prio
  }
  std::cout << "    end coro(" << max << ")\n";
}
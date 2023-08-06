#include "coropool.hpp"
#include <iostream>
#include <syncstream>   // for std::osyncstream

inline auto syncOut(std::ostream& strm = std::cout) {
  return std::osyncstream{strm};
}

CoroPoolTask print(std::string id, std::string msg)
{
  syncOut() << "    > " << id <<  " print: " << msg
            << "   on thread: " << std::this_thread::get_id() << std::endl;
  co_return;  // make it a coroutine
}

CoroPoolTask runAsync(std::string id)
{
  syncOut() << "===== " << id << " start     "
            << "   on thread: " << std::this_thread::get_id() << std::endl;

  co_await print(id + "a", "start");
  syncOut() << "===== " << id << " resume    "
            << "   on thread " << std::this_thread::get_id() << std::endl;

  co_await print(id + "b", "end  ");
  syncOut() << "===== " << id << " resume    "
            << "   on thread " << std::this_thread::get_id() << std::endl;

  syncOut() << "===== " << id << " done" << std::endl;
}
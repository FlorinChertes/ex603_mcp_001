#include "../inc/coro/coro_parser.hpp"

#include "../inc/coro/coro_return_type.hpp"

#include "../inc/coro/corochat.hpp"

#include "../inc/coro/coroasync.hpp"
#include "../inc/coro/coroprio.hpp"

//#include "../inc/coro/coro_iter.hpp"

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

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
generator<int> counter(int start, int end)
{
    while (start < end) {
        // Yielding a value and giving control back to the caller
        co_yield start;
        ++start;
    }
}

void test_UseCounter_065_02()
{
    std::cout << "\n*** test UseCounter 065_02 ***" << std::endl;
    std::cout << '\n';

    auto g = counter(1, 5);

    //This sequence runs from 1 to 5
    for (auto i : g) {
        std::cout << "Use Counter Value (" << i << ")\n";
    }
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_UpdateCoro_065_03()
{
    std::cout << "\n*** test Update coro 065_03 ***" << std::endl;
    std::cout << '\n';

    CoroPrioScheduler sched;

    std::cout << "schedule coroutines\n";
    sched.start(coro_prio(5));
    sched.start(coro_prio(1));
    sched.start(coro_prio(4));

    std::cout << "loop until all are processed\n";
    while (sched.resumeNext()) {
        std::cout << "resume next" << '\n';
    }

    std::cout << '\n';
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_CoroPool_065_04()
{
    std::cout << "\n*** test start Coro Pool 065_04 ***" << std::endl;
    std::cout << '\n';

    // init pool of coroutine threads:
    syncOut() << "main() start on thread " << std::this_thread::get_id() << std::endl;
    CoroPool pool{ 4 };

    // start main coroutine and run it in coroutine pool:
    //syncOut() << "main() runTask(runAsync(1)) " << std::this_thread::get_id() << std::endl;
    //CoroPoolTask t1 = runAsync("1");
    //pool.runTask(std::move(t1));

    // start multiple coroutines and run them in coroutine pool:
    for (int i = 1; i <= 4; ++i) {
        syncOut() << "runTask(runAsync(" << i << "))" << std::endl;
        pool.runTask(runAsync(std::to_string(i)));
    }

    // wait until all coroutines are done:
    syncOut() << "\nmain() waitUntilNoCoros() thread: " << std::this_thread::get_id() << std::endl;
    pool.waitUntilNoCoros();

    syncOut() << "main() end on thread " << std::this_thread::get_id() << std::endl;

    std::cout << "\n*** test end Coro Pool 065_04 ***" << std::endl;
    std::cout << '\n';
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

Chat Fun()
{
    using namespace std::literals;

    co_yield "Hello!\n"s;                   // call promise_type.yield_value

    std::cout << co_await std::string{};    // call promise_type.await_transform

    co_return "Here!\n"s;                   // call promise_type.return_value
}

void test_CoroChat_065_05()
{
    using namespace std::literals;

    std::cout << "\n*** test start Coro Chat 065_05 ***" << std::endl;
    std::cout << '\n';


    Chat chat = Fun();

    std::cout << chat.listen();             // Trigger FSM

    chat.answer("Where are you?\n"s);       // Sent data into the coroutine

    std::cout << chat.listen();             // Wait for more data from the coroutine


    std::cout << "\n*** test end Coro Chat 065_05 ***" << std::endl;
    std::cout << '\n';
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

ReturnType hello_coroutine()
{
    std::cout << "Hello from coroutine" << std::endl;

    const int send_value{42};
    std::cout << "Hello from coroutine, before send value: " << send_value << std::endl;

    const int get_value = co_await ReturnType::TheAwaiter{ send_value };

    std::cout << "Hello from coroutine, after get value: " << get_value << std::endl;
}

void test_CoroReturnType_065_06()
{
    using namespace std::literals;

    std::cout << "\n*** test start Coro Return Type 065_06 ***" << std::endl;
    std::cout << '\n';

    ReturnType returnType = hello_coroutine();

    returnType.resume();
    const int value_from_coroutine{ returnType.get_value() };
    std::cout << "get value from coroutine: " << value_from_coroutine << std::endl;

    const int value_to_coroutine{ 13 };
    std::cout << "set value to  coroutine: " << value_to_coroutine << std::endl;
    returnType.set_value(value_to_coroutine);
    returnType.resume();

    std::cout << "\n*** test end Coro Return Type 065_06 ***" << std::endl;
    std::cout << '\n';
}

void test_CoroReturnType_065_07()
{
    using namespace std::literals;

    std::cout << "\n*** test start Coro Parser 065_07 ***" << std::endl;
    std::cout << '\n';

    // #A Create the Parse coroutine and store the handle in p
    auto fsm = Parse();

    std::vector<byte> fakeBytes1{
      0x70_B, 0x08_B, 0x09_B, ESC, SOF, ESC,
      'H'_B, 'e'_B, 'l'_B, 'l'_B, 'o'_B, ESC, SOF,
      0x7_B, 0x08_B, 0x09_B, ESC, SOF, 'n'_B, 'i'_B, 'c'_B, 'e'_B, ESC, SOF,
      0x08_B, 0x09_B, ESC, SOF, 'W'_B, 'o'_B, 'r'_B };
    // #B Simulate the first network stream
    auto stream_1 = sender(std::move(fakeBytes1));
    ProcessStream(stream_1, fsm);  // #C Process the bytes

    // #D Simulate the reopening of the network stream
    std::vector<byte> fakeBytes2{
         'l'_B, 'd'_B, ESC, SOF, 0x99_B };
    // #E Simulate a second network stream
    auto stream_2 = sender(std::move(fakeBytes2));
    // #F We still use the former p and feed it with new bytes
    ProcessStream(stream_2, fsm);

    std::cout << "\n*** test end Coro Parser 065_07 ***" << std::endl;
    std::cout << '\n';
}

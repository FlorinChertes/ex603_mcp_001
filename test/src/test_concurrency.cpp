
#include <array>
#include <vector>
#include <string>

#include <iomanip>
#include <sstream>
#include <iostream>

#include <atomic>

#include <syncstream>
#include <thread>

#include <chrono>
#include <memory>

#include <cassert>

// spinLock.cpp
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class Spinlock {
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
public:
    
    void lock() {
        while (flag.test_and_set());
    }
    
    void unlock() {
        flag.clear();
    }
};

int idx{};
Spinlock spin{};


void workOnResource() {

    spin.lock();

    // shared resource
    idx++;
    std::cout << "idx = " << idx << '\n';

    spin.unlock();
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_052()
{
    std::cout << "*** test 052 ***" << std::endl;
    std::thread t(workOnResource);
    std::thread t2(workOnResource);

    t.join();
    t2.join();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::vector<int> vec{};
std::atomic_flag atomicFlag{};

void prepareWork() {
    vec.insert(vec.end(), { 0, 1, 0, 3 });
    std::cout << "Sender: Data prepared.\n";

    atomicFlag.test_and_set();
    atomicFlag.notify_one();
}

void completeWork() {
    std::cout << "Waiter: Waiting for data.\n";
    atomicFlag.wait(false);

    vec[2] = 2;
    std::cout << "Waiter: Complete the work.\n";

    for (auto i : vec)
        std::cout << i << " ";
    std::cout << '\n';
}

void test_053()
{
    std::cout << "*** test 053 ***" << std::endl;
    std::thread t1(prepareWork);
    std::thread t2(completeWork);

    t1.join();
    t2.join();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_054()
{
    std::cout << "*** test 054 ***" << std::endl;

    std::cout << '\n';
#if _MSC_VER
    std::atomic<std::shared_ptr<std::string>> sharString(
        std::make_shared<std::string>("Zero"));

    std::thread t1([&sharString] {
        sharString.store(std::make_shared<std::string>(*sharString.load() + "One"));
    });
    std::thread t2([&sharString] {
        sharString.store(std::make_shared<std::string>(*sharString.load() + "Two"));
    });
    std::thread t3([&sharString] {
        sharString.store(std::make_shared<std::string>(*sharString.load() + "Three"));
    });
    std::thread t4([&sharString] {
        sharString.store(std::make_shared<std::string>(*sharString.load() + "Four"));
    });
    std::thread t5([&sharString] {
        sharString.store(std::make_shared<std::string>(*sharString.load() + "Five"));
    });

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    std::cout << *sharString.load() << '\n';
#endif
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template < typename T>
T fetch_mult(std::atomic<T>& shared, T mult) {
    T oldValue = shared.load();
    while (!shared.compare_exchange_strong(oldValue, oldValue * mult));
    return oldValue;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_055()
{

    std::cout << "*** test 055 ***" << std::endl;

    std::atomic<int> value_{ 5 };

    auto func = [&value_] (){

        std::cout << value_ << '\n';
        fetch_mult(value_, 5);
        std::cout << value_ << '\n';
    };

    std::thread t1(func);
    std::thread t2(func);
    std::thread t3(func);
    std::thread t4(func);
    std::thread t5(func);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::string work;
std::atomic<bool> ready(false);

void consumer() {
    while (!ready.load()) {}
    std::cout << work << '\n';
}

void producer() {
    work = "done";
    ready = true;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_056()
{
    std::cout << "*** test 056 ***" << std::endl;

    std::thread con(consumer);
    std::thread prod(producer);

    prod.join();
    con.join();
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::atomic<int> atom{ 0 };
int somethingShared{ 0 };

void writeShared() {
    somethingShared = 2011;
    atom.store(3, std::memory_order_release);
}

void readShared() {
    using namespace std::chrono_literals;
    while (!(atom.fetch_sub(1, std::memory_order_acquire) > 0)) {
        std::this_thread::sleep_for(100ms);
    }

    std::osyncstream(std::cout) << "somethingShared: " <<
        "111111111 " <<
        "222222222 " <<
        "333333333 " <<
        somethingShared << '\n';
}

void test_057()
{
    std::cout << "*** test 057 ***" << std::endl;

    std::thread t2(readShared);
    std::thread t3(readShared);
    std::thread t4(readShared);

    std::thread t1(writeShared);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    std::cout << "atom: " << atom << '\n';
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

std::atomic<int> count = { 0 };

void add()
{
    for (int n = 0; n < 100000; ++n) {
        count.fetch_add(1/*, std::memory_order_relaxed*/);
     }
}

void test_058()
{
    const auto start = std::chrono::steady_clock::now();

    std::vector<std::jthread> v;
    for (int n = 0; n < 100; ++n) {
        v.emplace_back(add);
    }
    for (auto& t : v) {
        t.join();
    }

    const auto end = std::chrono::steady_clock::now();
    const auto res = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Final counter value is " << count << '\n';
    std::cout << "time: " << res << " ms\n";
}

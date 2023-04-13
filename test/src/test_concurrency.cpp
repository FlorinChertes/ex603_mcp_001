
#include <atomic>
#include <thread>

#include <array>
#include <vector>
#include <string>

#include <iomanip>
#include <sstream>
#include <iostream>
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
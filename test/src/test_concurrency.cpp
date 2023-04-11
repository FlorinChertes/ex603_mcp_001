
#include <atomic>
#include <thread>

#include <array>
#include <vector>
#include <string>

#include <iomanip>
#include <sstream>
#include <iostream>

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

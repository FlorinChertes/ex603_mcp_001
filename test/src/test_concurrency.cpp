
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

Spinlock spin;

void workOnResource() {
    static int idx{};

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

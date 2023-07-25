
#include <chrono>
#include <cstddef>

#include <iomanip>
#include <iostream>

#include <array>
#include <list>
#include <vector>

#include <memory_resource>

template<typename Func>
auto benchmark(Func test_func, int iterations)
{
    const auto start = std::chrono::system_clock::now();
    while (iterations-- > 0)
        test_func();
    const auto stop = std::chrono::system_clock::now();
    const auto secs = std::chrono::duration<double>(stop - start);
    return secs.count();
}



void test_pmr_066()
{
    char buffer[64] = {}; // a small buffer on the stack
    std::fill_n(std::begin(buffer), std::size(buffer) - 1, '_');
    std::cout << buffer << '\n';

    std::pmr::monotonic_buffer_resource pool{std::data(buffer), std::size(buffer)};

    std::pmr::vector<char> vec{ &pool };
    for (char ch = 'a'; ch <= 'z'; ++ch)
        vec.push_back(ch);

    std::cout << buffer << '\n';
}
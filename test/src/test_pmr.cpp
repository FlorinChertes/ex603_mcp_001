
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

void test_pmr_066_01()
{
    std::cout << "\n*** test 066_01 ***" << std::endl;
    std::cout << '\n';

    constexpr int iterations{ 100 };
    constexpr int total_nodes{ 20'000 };

    auto default_std_alloc = []
    {
        std::list<int> list;
        for (int i{}; i != total_nodes; ++i)
            list.push_back(i);
    };

    auto default_pmr_alloc = []
    {
        std::pmr::list<int> list;
        for (int i{}; i != total_nodes; ++i)
            list.push_back(i);
    };

    auto pmr_alloc_no_buf = []
    {
        std::pmr::monotonic_buffer_resource mbr;
        std::pmr::polymorphic_allocator<int> pa{&mbr};
        std::pmr::list<int> list{pa};
        for (int i{}; i != total_nodes; ++i)
            list.push_back(i);
    };

    auto pmr_alloc_and_buf = []
    {
        std::array<std::byte, total_nodes * 32> buffer; // enough to fit in all nodes
        std::pmr::monotonic_buffer_resource mbr{buffer.data(), buffer.size()};
        std::pmr::polymorphic_allocator<int> pa{&mbr};
        std::pmr::list<int> list{pa};
        for (int i{}; i != total_nodes; ++i)
            list.push_back(i);
    };

    const double t1 = benchmark(default_std_alloc, iterations);
    const double t2 = benchmark(default_pmr_alloc, iterations);
    const double t3 = benchmark(pmr_alloc_no_buf, iterations);
    const double t4 = benchmark(pmr_alloc_and_buf, iterations);

    std::cout << std::fixed << std::setprecision(3)
        << "t1 (default std alloc): " << t1 << " sec; t1/t1: " << t1 / t1 << '\n'
        << "t2 (default pmr alloc): " << t2 << " sec; t1/t2: " << t1 / t2 << '\n'
        << "t3 (pmr alloc  no buf): " << t3 << " sec; t1/t3: " << t1 / t3 << '\n'
        << "t4 (pmr alloc and buf): " << t4 << " sec; t1/t4: " << t1 / t4 << '\n'
        ;
}

void test_pmr_066()
{
    std::cout << "\n*** test 066 ***" << std::endl;

    char buffer[64] = {}; // a small buffer on the stack
    std::fill_n(std::begin(buffer), std::size(buffer) - 1, '_');
    std::cout << buffer << '\n';

    std::pmr::monotonic_buffer_resource pool{std::data(buffer), std::size(buffer)};

    for (int i{}; i < 64; ++i)
    {
        std::cout << buffer[i];
    }
    std::cout << '\n';

    std::pmr::vector<char> vec{ &pool };

    for (char ch = 'a'; ch <= 'z'; ++ch)
        vec.push_back(ch);

    for (int i{}; i < 64; ++i)
    {
        std::cout << buffer[i];
    }
    std::cout << '\n';

    std::cout << buffer << '\n';

    std::cout << "\n*** test 066 end ***" << std::endl;
}

void test_pmr_067()
{
    std::cout << "\n*** test 067 ***" << std::endl;

    constexpr std::size_t buf_len{64};

    std::array<char, buf_len> buffer;
    std::fill_n(std::begin(buffer), std::size(buffer), '_');
    std::pmr::monotonic_buffer_resource pool{buffer.data(), buffer.size()};
    for (std::size_t i{}; i < buf_len; ++i)
    {
        std::cout << buffer[i];
    }
    std::cout << '\n';

    std::pmr::vector<char> vec{ &pool };
    for (char ch = 'a'; ch <= 'z'; ++ch)
    {
        vec.push_back(ch);
    }
    for (std::size_t i{}; i < buf_len; ++i)
    {
        std::cout << buffer[i];
    }
    std::cout << '\n';

    std::cout << "\n*** test 067 end ***" << std::endl;
}

void test_pmr_068()
{
    std::cout << "sizeof(std::string): " << sizeof(std::string) << '\n';
    std::cout << "sizeof(std::pmr::string): " << sizeof(std::pmr::string) << '\n';

    char buffer[256] = {}; // a small buffer on the stack
    std::fill_n(std::begin(buffer), std::size(buffer) - 1, '_');

    const auto BufferPrinter = [](std::string_view buf, std::string_view title) {
        std::cout << title << ":\n";
        for (auto& ch : buf) {
            std::cout << (ch >= ' ' ? ch : '#');
        }
        std::cout << '\n';
    };

    BufferPrinter(buffer, "zeroed buffer");

    std::pmr::monotonic_buffer_resource pool{std::data(buffer), std::size(buffer)};
    std::pmr::vector<std::pmr::string> vec{ &pool };
    vec.reserve(5);

    vec.push_back("Hello World");
    vec.push_back("One Two Three");
    BufferPrinter(std::string_view(buffer, std::size(buffer)), "after two short strings");

    vec.emplace_back("This is a longer string");
    BufferPrinter(std::string_view(buffer, std::size(buffer)), "after longer string strings");

    vec.push_back("Four Five Six");
    BufferPrinter(std::string_view(buffer, std::size(buffer)), "after the last string");
}

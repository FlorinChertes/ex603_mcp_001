
#include <numeric>

#if _MSC_VER
    #include <format>
#endif

#include <functional>
#include <algorithm>
#include <string>

#include <iostream>
#include <cassert>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_032()
{
    std::cout << "*** test 032 ***" << std::endl;

    std::cout << std::plus<int>{}(1, 1.23) << '\n';
    std::cout << std::plus<>{}(1, 1.23) << '\n';

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template<typename T, typename U>
auto simple_plus(T lhs, U rhs) -> decltype(lhs + rhs)
{
    return lhs + rhs;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_033()
{
    std::cout << "*** test 033 ***" << std::endl;

    std::cout << simple_plus<int>(1, 1.23) << '\n';
    std::cout << simple_plus<>(1, 1.23) << '\n';

    //auto x = int{ 1.5 };
    int x = 1.5;
    auto data = std::vector{ 1 };
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::vector<int> get_next_row(const std::vector<int>& last_row)
{
    std::vector next_row{ 1 };
    if (last_row.empty())
    {
        return next_row;
    }
    
    for (size_t idx = 0; idx + 1 < last_row.size(); ++idx)
    {
        next_row.emplace_back(last_row[idx] + last_row[idx + 1]);
    }
    next_row.emplace_back(1);

    return next_row;
}

auto generate_triangle(int rows)
{
    std::vector<std::vector<int>> triangle{ {1} };
    for (int row = 0; row < rows; ++row)
    {
        triangle.push_back(get_next_row(triangle.back()));
    }
    return triangle;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template<typename T>
std::ostream& operator << (std::ostream& s, const std::vector<std::vector<T>>& triangle)
{
    for (const auto& row : triangle)
    {
        std::ranges::copy(row, std::ostream_iterator<T>(s, " "));
        s << '\n';
    }

    return s;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_034()
{
    std::cout << "*** test 034 ***" << std::endl;

    const auto rows_count {6};
    const auto pascal_triangle { generate_triangle(rows_count) };
    std::cout << pascal_triangle;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void show_vectors(std::ostream& s,
    const std::vector<std::vector<int>>& v)
{
    const size_t final_row_size = v.back().size();
    std::string spaces(final_row_size * 3, ' ');
    for (const auto& row : v)
    {
        s << spaces;
        if (spaces.size() > 3)
            spaces.resize(spaces.size() - 3);

        for (const auto& data : row)
        {

#if _MSC_VER
            s << std::format("{: ^{}}", data, 6);
#else
            s << data << ' '
#endif

        }
        s << '\n';
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void check_properties(const std::vector<std::vector<int>>& triangle)
{
    size_t row_number = 1;
    for (const auto& row : triangle)
    {
        assert(row.front() == 1);
        assert(row.back() == 1);
        assert(row.size() == row_number++);
    }

    int expected_total = 1;
    for (const auto& row : triangle)
    {
        assert(std::accumulate( row.begin(),
                                row.end(),
                                0) == expected_total);

            expected_total *= 2;
    }

    for (const auto& row : triangle)
    {
        assert(std::all_of( row.begin(),
                            row.end(),
                            [](int x) { return x >= 0; }));
    }

    for (const auto& row : triangle)
    {
        assert(std::ranges::all_of(row, [](auto x) { return x >= 0; }));
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_035()
{
    std::cout << "*** test 035 ***" << std::endl;

#if _MSC_VER
    std::cout << std::format("{: ^{}}", 213, 24);
    std::cout << '\n';
#endif
    const auto rows_count{ 6 };
    const auto pascal_triangle{ generate_triangle(rows_count) };
    check_properties(pascal_triangle);

    show_vectors(std::cout, pascal_triangle);

}

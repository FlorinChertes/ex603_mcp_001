#include <functional>

#include <cstddef>
#include <algorithm>
#include <iostream>
#include <numbers>
#include <string>

#include <iostream>


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
    std::vector<int> data;

    std::vector<std::vector<int>> triangle;
    for (int row = 0; row < rows; ++row)
    {
        data = get_next_row(data);
        triangle.push_back(data);
    }
    
    return triangle;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_034()
{
    std::cout << "*** test 034 ***" << std::endl;

    const auto rows_count {6};
    const auto pascal_triangle{ generate_triangle(rows_count) };
    for (const auto& row : pascal_triangle)
    {
        for (const auto& row_elem : row)
        { 
            std::cout << row_elem << " ";
        }
        std::cout << '\n';
    }
}

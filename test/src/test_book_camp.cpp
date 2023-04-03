
#include <optional>
#include <numeric>

#if _MSC_VER
    #include <format>
#endif

#include <ranges>

#include <functional>
#include <algorithm>

#include <vector>
#include <string>

#include <sstream>
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

    [[maybe_unused]] auto data = std::vector{ 1 };
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
            s << data << ' ';
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
        assert(std::ranges::all_of( row,
            [](int x) { return x >= 0; }) == true);
    }

    std::ranges::for_each(triangle,
        [](const auto row) {
            assert(std::ranges::all_of(row,
                [](int x) { return x >= 0; }) == true);
        }
    );

    for (const auto& row : triangle)
    {
        auto&& negatives = row | std::views::filter(
            [](int x) { return x < 0; });
        assert(negatives.empty() == true);
    }

    std::ranges::for_each(triangle,
        [](const auto& row) {

        auto&& negatives = row | std::views::filter(
            [](int x) { return x < 0; });
        assert(negatives.empty() == true);
        }
    );

    auto is_palindrome = [](const std::vector<int>& v) {
        const auto forward = v | std::views::take(v.size() / 2);
        const auto backward = v | std::views::reverse
            | std::views::take(v.size() / 2);
        return std::ranges::equal(forward, backward);
        };
    assert(std::ranges::all_of(triangle, is_palindrome) == true);

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void show_view(std::ostream& s,
    const std::vector<std::vector<int>>& v)
{
    std::string spaces(v.back().size(), ' ');
    for (const auto& row : v)
    {
        s << spaces;
        if (spaces.size())
            spaces.resize(spaces.size() - 1);
        auto odds = row | std::views::transform([](int x)
            { return x % 2 ? '*' : ' '; });
        for (const auto& data : odds)
        {
            s << data << ' ';
        }
        s << '\n';
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_035()
{
    std::cout << "*** test 035 ***" << std::endl;

#if _MSC_VER
    std::cout << std::format("{: ^{}}", 543, 24);
    std::cout << '\n';
#endif
    const auto rows_count{ 6 };
    const auto pascal_triangle{ generate_triangle(rows_count) };
    check_properties(pascal_triangle);

    show_vectors(std::cout, pascal_triangle);
    show_view(std::cout, pascal_triangle);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_036()
{
    std::cout << "*** test 036 ***" << std::endl;

    std::istringstream s("1 2 3 error");
    int n;

    std::cout << std::boolalpha << "s is " << static_cast<bool>(s) << '\n';
    while (s >> n)
        std::cout << n << '\n';
    std::cout << "s is " << static_cast<bool>(s) << '\n';
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
unsigned some_const_number()
{
    return 42;
}

unsigned input_number()
{
    std::istringstream s("42 2 3 error");

    unsigned number;
    while (!(s >> number))
    {
        std::cout << "Please enter a number.\n>";
    }

    return number;
}

void guess_number(unsigned number)
{
    std::cout << "Guess the number.\n>";
    unsigned guess = input_number();
    if(guess != number)
    {
        std::cout << guess << " is wrong. Try again\n>";
     }
    else
        std::cout << guess  << " is right, well done.\n";
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_037()
{
    std::cout << "*** test 037 ***" << std::endl;

    guess_number(some_const_number());
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::optional<int> read_number(std::istream& in)
{
    int result{};
    if (in >> result)
    {
        return result;
    }

    in.clear();
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return {};
}

void guess_number_or_give_up(int number)
{
    std::istringstream s("51 76 3 The-End");

    std::cout << "Guess the number.\n>";
    std::optional<int> guess;

    while ((guess = read_number(s)))
    {
        if (guess.value() == number)
        {
            std::cout << number << " is the numeber, well done!";
            return;
        }
        std::cout << guess.value() << " is wrong. Try again\n>";
    }
    std::cout << "The number was " << number << "\n";
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_038()
{
    std::cout << "*** test 038 ***" << std::endl;

    guess_number_or_give_up(some_const_number());
}

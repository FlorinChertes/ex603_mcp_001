#include <thread>

#include <chrono>

#include <random>
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

#include <iomanip>
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

#if !defined(__clang__)
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
#endif
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#if !defined(__clang__)
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
#endif
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
#if !defined(__clang__)
    show_view(std::cout, pascal_triangle);
#endif
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
int some_const_number()
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
            std::cout << number << " is the number, well done!";
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

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void guess_number_with_clues(int number,
    std::invocable<int, int> auto message)
{
    std::istringstream s("12345 51 76 3 The-End");

    std::cout << "Guess the number.\n>";
    std::optional<int> guess;

    while ((guess = read_number(s)))
    {
        if (guess.value() == number)
        {
            std::cout << number << " is the number, well done!";
            return;
        }
        std::cout << message(number, guess.value()) << '>';
    }
#if _MSC_VER
    std::cout << std::format("The number was {}\n", number);
#else
    std::cout << "The number was " << number << '\n';
#endif
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_039()
{
    std::cout << "*** test 039 ***" << std::endl;

    auto make_message = [](int number, int guess) {
#if _MSC_VER
        return std::format("Your guess was too {}\n", (guess < number ? "small" : "big"));
#else
        return std::string("Your guess was too ")
            + std::string(guess < number ? "small\n" : "big\n");
#endif
    };
    guess_number_with_clues(some_const_number(), make_message);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int some_random_number()
{
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution dist(1, 100);
    return dist(engine);
}
void test_040()
{
    std::cout << "*** test 040 ***" << std::endl;
    std::cout << "random number: " << some_random_number() << '\n';
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
constexpr bool is_prime(int n)
{
    if (n == 2 || n == 3)
        return true;

    if (n <= 1 || n % 2 == 0 || n % 3 == 0)
        return false;

    for (int i = 5; i * i <= n; ++i)
    {
        if (n % i == 0)
            return false;
    }
    return true;
}
constexpr void check_properties()
{
    static_assert(is_prime(5) == true);
    static_assert(is_prime(6) == false);
    static_assert(is_prime(7) == true);
    static_assert(is_prime(8) == false);
}

void test_041()
{
    std::cout << "*** test 041 ***" << std::endl;
    check_properties();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int some_prime_number()
{
    std::random_device rd;
    std::mt19937 engine{ rd() };
    std::uniform_int_distribution<int> dist{ 1, 99999 };

    int n{};
    while ((n = dist(engine)) && !is_prime(n))
        ;

    return n;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_042()
{
    std::cout << "*** test 042 ***" << std::endl;

    auto make_message = [](int number, int guess) {
#if _MSC_VER
        return std::format("Your guess was too {}\n", (guess < number ? "small" : "big"));
#else
        return std::string("Your guess was too ")
            + std::string(guess < number ? "small\n" : "big\n");
#endif
    };
    guess_number_with_clues(some_prime_number(), make_message);

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::string check_which_digits_correct(int number, int guess)
{
#if _MSC_VER
    auto ns = std::format("{:0>5}", (number));
    auto gs = std::format("{:0>5}", (guess));
#else
    std::ostringstream n;
    n << std::setfill('0') << std::setw(5) << number;
    std::string ns = n.str();

    std::ostringstream g;
    g << std::setfill('0') << std::setw(5) << guess;
    const std::string gs = g.str();
#endif
    std::string matches(5, '.');

    for (size_t i = 0, stop = gs.length(); i < stop; ++i)
    {
        char guess_char = gs[i];
        if (i < ns.length() && guess_char == ns[i])
        {
            matches[i] = '*';
            ns[i] = '*'; // don't reuse this digit
        }
    }

    for (size_t i = 0, stop = gs.length(); i < stop; ++i)
    {
        char guess_char = gs[i];
        if (i < ns.length() && ns[i] != '*')
        {
            if (size_t idx = ns.find(guess_char, 0);
                idx != std::string::npos)
            {
                matches[i] = '^';
                ns[idx] = '^'; // don't reuse this digit
            }
        }
    }

    return matches;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_043()
{
    std::cout << "*** test 043 ***" << std::endl;


    assert(check_which_digits_correct(12347, 23471) == "^^^^^");

    auto got = check_which_digits_correct(12347, 11779);
    assert(got == "*.^..");
    got = check_which_digits_correct(12345, 23451);
    assert(got == "^^^^^");
    got = check_which_digits_correct(12345, 12345);
    assert(got == "*****");
    got = check_which_digits_correct(48533, 12345);
    assert(got == "..^^^");
    got = check_which_digits_correct(98041, 41141);
    assert(got == "...**");
    assert(is_prime(17231));

    got = check_which_digits_correct(1723, 17231);
    assert(got == "^^^^.");

    got = check_which_digits_correct(17231, 1723);
    assert(got == ".^^^^");

    unsigned number = 78737;
    got = check_which_digits_correct(number, 87739);
    assert(got == "^^**.");

    got = check_which_digits_correct(5081, 53);
    assert(got == "*.*^.");

    got = check_which_digits_correct(5081, 79);
    assert(got == "*.*..");
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_044()
{
    std::cout << "*** test 044 ***" << std::endl;

    auto message = [](int number, int guess) {
#if _MSC_VER
        auto gs = std::format("{:0>5}", (guess));
        return std::format("{}, {}, {} \n", number, gs, check_which_digits_correct(number, guess));
#else
    std::ostringstream n;
    n << std::setfill('0') << std::setw(5) << number;
    std::string ns = n.str();

    std::ostringstream g;
    g << std::setfill('0') << std::setw(5) << guess;
    const std::string gs = g.str();

    return ns + std::string{ ", " } + gs + std::string{ ", " } + check_which_digits_correct(number, guess) + '\n';
#endif
    };

    guess_number_with_clues(some_prime_number(), message);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void guess_number_with_more_clues(int number, auto messages)
{
    std::istringstream s("12345 53 79 3 The-End");

    std::cout << "Guess the number.\n>";
    std::optional<int> guess;

    while ((guess = read_number(s)))
    {
        if (guess.value() == number)
        {
            std::cout << number << " is the number, well done!";
            return;
        }
#if _MSC_VER
        std::cout << std::format("{:0>5} is wrong. Try again\n", guess.value());
#else
        std::ostringstream g;
        g << std::setfill('0') << std::setw(5) << guess.value();
        const std::string gs = g.str();
        std::cout << gs << " is wrong. Try again\n";
#endif

        for (auto message : messages)
        {
            auto clue = message(guess.value());
            if (clue.length())
            {
                std::cout << clue;
                break;
            }
        }
    }
#if _MSC_VER
    std::cout << std::format("The number was {}\n", number);
#else
    std::cout << "The number was " << number << '\n';
#endif
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_045()
{
    std::cout << "*** test 045 ***" << std::endl;

    check_properties();

    auto check_prime = [](int guess) {
        return std::string((is_prime(guess)) ? "" : "Not prime\n");
    };

    auto check_length = [](int guess) {
        return std::string((guess < 100000) ? "" : "Too long\n");
    };

    const int number = some_prime_number();
    auto check_digits = [number](int guess) {
#if _MSC_VER
        return std::format("{}\n", check_which_digits_correct(number, guess));
#else
        return std::string{ check_which_digits_correct(number, guess) } + '\n';
#endif
    };

    std::vector <std::function<std::string(int)> > messages
    {
        check_length,
        check_prime,
        check_digits
    };

    guess_number_with_more_clues(number, messages);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Listing 4.2 Duration between two time points
void duration_to_end_of_year()
{
    std::chrono::time_point now = std::chrono::system_clock::now();
    constexpr auto year = 2022;
    auto new_years_eve = std::chrono::year_month_day(
        std::chrono::year(year),
        std::chrono::month(12),
        std::chrono::day(31)
    );
    auto event = std::chrono::sys_days(new_years_eve);
    std::chrono::duration dur = event - now;
#if _MSC_VER
    std::cout << dur << " until event\n";
#else
    std::cout << dur.count() << " until event\n";
#endif
    std::cout << std::ratio<3, 6>::num << '/' << std::ratio<3, 6>::den << '\n';
}

void countdown()
{
    using namespace std::chrono;

    time_point now = system_clock::now();
    const auto ymd = year_month_day{ floor<days>(now) };

    auto this_year = ymd.year();
    auto new_years_eve = this_year / December / 31;

    auto event = sys_days(new_years_eve);
    duration dur = event - now;

#if _MSC_VER
    std::cout << duration_cast<days>(dur) << " until event \n";
#else
    std::cout << std::chrono::duration_cast<std::chrono::days>(dur).count()
        << " days until event \n";
#endif
}

//Listing 4.9 A testable countdown
constexpr
std::chrono::system_clock::duration countdown(
    std::chrono::system_clock::time_point start)
{
    using namespace std::chrono;

    auto days_only = floor<days>(start);

    const auto ymd = year_month_day{ days_only };

    auto this_year = ymd.year();
    auto new_years_eve = this_year / December / last;

    auto event = sys_days(new_years_eve);
    return event - start;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_046()
{
    std::cout << "*** test 046 ***" << std::endl;
    duration_to_end_of_year();
    countdown();

    auto result = countdown(std::chrono::system_clock::now());

#if _MSC_VER
    std::cout << duration_cast<std::chrono::days>(result) << " until event \n";
#else
    std::cout << duration_cast<std::chrono::days>(result).count() << " days until event \n";
#endif
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Listing 4.10 Check the countdown function
constexpr
bool check_properties_chrono()
{
    using namespace std::chrono;

    constexpr auto new_years_eve = 2022y / December / last;
    constexpr auto one_day_away = sys_days{ new_years_eve } - 24h;

    constexpr auto duration = countdown(one_day_away);
    static_assert(duration_cast<days>(duration) == days{ 1 });

    return true;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_047()
{
    std::cout << "*** test 047 ***" << std::endl;

    //Listing 4.10
    static_assert(check_properties_chrono() == true);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_048()
{
    std::cout << "*** test 048 ***" << std::endl;

    using namespace std::chrono;

    for (int i = 0; i < 5; ++i)
    {
        std::this_thread::sleep_for(500ms);
        auto dur = countdown(system_clock::now());

#if _MSC_VER
        std::cout << duration_cast<seconds>(dur) << " until event\n";
#else
        std::cout << duration_cast<seconds>(dur).count() << " sec. until event\n";
#endif
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
constexpr std::chrono::system_clock::duration
countdown_to(std::chrono::system_clock::time_point now,
    std::chrono::year_month_day date)
{
    using namespace std::chrono;

    auto event = sys_days(date);
    return event - now;
}

std::optional<std::chrono::year_month_day>
read_date(std::istream& in)
{
    using namespace std::string_literals;

    auto format_str = "%Y-%m-%d"s;
    if (std::chrono::year_month_day date;
        in >> std::chrono::parse(format_str, date))
    {

        return date;
    }

    in.clear();
    std::cout << "Invalid format. Expected " <<
    format_str << '\n';
    return {};
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_049()
{
    std::cout << "*** test 049 ***" << std::endl;

    using namespace std::string_literals;
    using namespace std::chrono;

    auto today_now = system_clock::now();
    const auto days_only = floor<days>(today_now);
    const auto ymd = year_month_day{ days_only };

    const auto this_year  = static_cast<int>(ymd.year());
    const auto this_month = static_cast<unsigned int>(ymd.month());
    const auto this_day   = static_cast<unsigned int>(ymd.day());

    std::ostringstream o;
    o << this_year << "-12-25"s;
    const auto christmas_day{ o.str() };

    std::istringstream in(christmas_day);
    std::optional<std::chrono::year_month_day> event_date = read_date(in);
    if (event_date)
    {
        if (this_month == 12 && this_day > 25)
        {
            // 26.12. - 31.12. is Christmas!
            today_now = sys_days{ year{ this_year } / December / 25 };
        }
        const auto& dur = countdown_to(today_now, event_date.value());

        std::cout << duration_cast<days>(dur).count() <<
            " days until " << event_date.value() << "\n";
    }
}

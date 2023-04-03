
#include <ranges_func.h>

#include <algorithm>
#include <ranges>
#include <vector>

#include <string>
#include <iostream>

void test_ranges_026()
{
    std::cout << "*** test ranges 026 ***" << std::endl;
    try {
        std::vector<int> coll{ 0, 2, -3, 5, -1, 6, 8, -4, 9 };

        std::ranges::filter_view v1{ coll,
            [](auto elem) { return elem % 3 == 0; }
        };
        for (const auto& i : v1)
        {
            std::cout << i << ", ";
        }
        std::cout << std::endl;


        std::ranges::transform_view v2{ v1,
            [](auto elem) {return elem * elem;}
        };
        for (const auto& i : v2)
        {
            std::cout << i << ", ";
        }
        std::cout << std::endl;

        auto v3 = std::views::iota(5, 20)
            | std::views::filter([](auto elem) { return elem % 3 == 0; })
            | std::views::transform([](auto elem) {return elem * elem; })
            | std::views::take(3);

        for (const auto& i : v3)
        {
            std::cout << i << ", ";
        }
        std::cout << std::endl;
    }
    catch (const std::out_of_range&)
    {
        std::cout << "out_of_range exception" << std::endl;
    }
}


void test_ranges_027()
{
    std::cout << "*** test ranges 027 ***" << std::endl;

    auto vi = std::views::iota(5, 20);

    std::vector ve = to_vector(vi);

    std::copy(ve.begin(), ve.end(),						//source
        std::ostream_iterator<int>(std::cout, ", "));	//destination
    std::cout << std::endl;

}

void test_ranges_028()
{
    std::cout << "*** test ranges 028 ***" << std::endl;

    auto vi = std::views::iota(5, 20);

    const std::vector<int> ve = vi | std::vector<int>{};

    std::ranges::copy(ve.begin(), ve.end(),		        //source
        std::ostream_iterator<int>(std::cout, ", "));	//destination
    std::cout << std::endl;

}

void test_ranges_028_1()
{
    std::cout << "*** test ranges 028.1 ***" << std::endl;

    auto v = std::views::iota(0, 6) | std::vector<int>{};
    const auto new_end = std::ranges::remove_if(v, [](int i) { return i < 3; });

    std::ranges::copy(v,           		                //source
        std::ostream_iterator<int>(std::cout, ", "));	//destination
    std::cout << std::endl;

   std::ranges::copy(new_end,           		        //source
       std::ostream_iterator<int>(std::cout, ", "));	//destination
   std::cout << std::endl;
}

void test_ranges_028_2()
{
    std::cout << "*** test ranges 028.2 ***" << std::endl;

    const auto v = std::views::iota(0, 6) | std::vector<int>{};
    std::ranges::copy(v
        | std::views::drop_while([](int i) { return i < 3; }),  //source
        std::ostream_iterator<int>(std::cout, ", "));           //destination
    std::cout << std::endl;
}

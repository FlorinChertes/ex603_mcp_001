#include "../inc/gtest_ranges.h"

#include "../../lib/inc/ranges_func.h"

#include <array>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
 TEST(VectorTest, test_initialize_from_iota_001)
 {
	const auto vi = std::views::iota(5, 20);
 	std::vector ve = to_vector(vi);
 	EXPECT_EQ(ve.size(), 15);
 }

 //-----------------------------------------------------------------------------
 //-----------------------------------------------------------------------------
 TEST(VectorTest, test_initialize_from_iota_002)
 {
	 const auto vi = std::views::iota(5, 20);
	 const auto ve = vi | std::vector<int>{};
	 EXPECT_EQ(ve.size(), 15);
 }


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
TEST(RangesTest, test_for_each_ranges)
{
	const std::vector<Product> prods{
		{ "box", 10.0 }, {"tv", 100.0}, {"none", -1.0}
	};

	auto out = [](const auto& v) { std::cout << v << ", "; };

	// standard version:
	std::cout << "std::ranges::for_each: \n";
	std::ranges::for_each(begin(prods), end(prods), [](const Product& p) {
		std::cout << p.name_ << ", " << p.value_ << '\n';
		});

	 std::cout << "std::ranges::for_each only names reverse: \n";
	 std::ranges::for_each(prods | std::views::reverse,
	 	out, &Product::name_);

	 std::cout << '\n';
	 EXPECT_EQ(prods.size(), 3);
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
TEST(RangesTest, test_find_first_of_ranges)
{
	const std::vector<Product> prods{
		{ "box", 10.0 }, {"default", 0.0 }, {"tv", 100.0}, {"rocket", 10000.0},
		{ "car", 1000.0 }, {"toy", 40.0}, {"none", 0.0 }, { "ball", 40.0 }
	};

	const std::vector<Product> invalids{
		{"default", 0.0 }, {"none", 0.0 }
	};

	// ranges version:
	const std::array<std::string, 2> arrInvalids{ "default", "none" };

	auto it = std::ranges::find_first_of(prods, arrInvalids,
		std::ranges::equal_to{}, &Product::name_);
	EXPECT_NE(it, end(prods));
	 if (it != end(prods)) {
	 	const auto pos = std::distance(begin(prods), it);
	 	std::cout << "std::ranges::find_first_of: " << it->name_
	 		<< " at: " << pos << '\n';

	 	auto it2 = std::ranges::find_first_of(prods | std::views::drop(pos + 1), arrInvalids,
	 		std::ranges::equal_to{}, &Product::name_);
	 	EXPECT_NE(it2, prods.end());
	 	if (it2 != end(prods))
	 		std::cout << "std::ranges::find_first_of: " << it2->name_
	 		<< " at: " << std::distance(begin(prods), it2) << '\n';
	 }
}

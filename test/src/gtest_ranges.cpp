#include "../../lib/inc/ranges_func.h"


#include <gmock/gmock.h>
#include <gtest/gtest.h>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
TEST(VectorTest, test_initialize_from_iota_001)
{
	auto vi = std::views::iota(5, 20);

	std::vector ve = to_vector(vi);

	EXPECT_EQ(ve.size(), 15);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
TEST(RangesTest, test_all_of)
{
	const std::vector nums = { 1, 2, 3, -4, 5, 6, 7, 8 };

	auto is_positive = [](const auto& v) { return v > 0; };

	// standard version:
	auto res = std::all_of(begin(nums), end(nums), is_positive);
	std::cout << "std::all_of: " << res << '\n';
	EXPECT_EQ(res, false);

	res = std::any_of(begin(nums), end(nums), is_positive);
	std::cout << "std::any_of: " << res << '\n';
	EXPECT_EQ(res, true);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
TEST(RangesTest, range_test_all_of)
{
	const std::vector nums = { 1, 2, 3, -4, 5, 6, 7, 8 };

	auto is_positive = [](const auto& v) { return v > 0; };

	// standard version:
	auto res = std::ranges::all_of(begin(nums), end(nums), is_positive);
	std::cout << "std::all_of: " << res << '\n';
	EXPECT_EQ(res, false);

	res = std::ranges::any_of(begin(nums), end(nums), is_positive);
	std::cout << "std::any_of: " << res << '\n';
	EXPECT_EQ(res, true);
}

struct Product {
	std::string name_;
	double value_{ 0.0 };
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
TEST(RangesTest, test_all_of_complex)
{
	const std::vector<Product> prods{
		{ "box", 10.0 }, {"tv", 100.0}, {"none", -1.0}
	};

	auto is_positive_val = [](const Product& p) {
		return p.value_ > 0;
	};


	// standard version:
	auto res = std::all_of(begin(prods), end(prods), is_positive_val);
	std::cout << "std::all_of: " << res << '\n';
	EXPECT_EQ(res, false);

	res = std::any_of(begin(prods), end(prods), is_positive_val);
	std::cout << "std::any_of: " << res << '\n';
	EXPECT_EQ(res, true);
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
TEST(RangesTest, range_test_all_of_complex)
{
	const std::vector<Product> prods{
		{ "box", 10.0 }, {"tv", 100.0}, {"none", -1.0}
	};

	auto is_positive = [](const auto& v) { return v > 0; };

	// standard version:
	auto res = std::ranges::all_of(begin(prods), end(prods), is_positive, &Product::value_);
	std::cout << "std::all_of: " << res << '\n';
	EXPECT_EQ(res, false);

	res = std::ranges::any_of(begin(prods), end(prods), is_positive, &Product::value_);
	std::cout << "std::any_of: " << res << '\n';
	EXPECT_EQ(res, true);
}


#include "../../lib/inc/ranges_func.h"

#include <array>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// TEST(VectorTest, test_initialize_from_iota_001)
// {
// 	auto vi = std::views::iota(5, 20);

// 	std::vector ve = to_vector(vi);

// 	EXPECT_EQ(ve.size(), 15);
// }

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

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
TEST(RangesTest, test_for_each)
{
	const std::vector<Product> prods{
		{ "box", 10.0 }, {"tv", 100.0}, {"none", -1.0}
	};

	// standard version:
	std::cout << "std::for_each: \n";
	std::for_each(begin(prods), end(prods), [](const Product& p) {
		std::cout << p.name_ << ", " << p.value_ << '\n';
		});

	std::cout << "std::for_each only names reverse: \n";
	std::for_each(rbegin(prods), rend(prods), [](const Product& p) {
		std::cout << p.name_ << '\n';
		});

	EXPECT_EQ(prods.size(), 3);
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

	// std::cout << "std::ranges::for_each only names reverse: \n";
	// std::ranges::for_each(prods | std::views::reverse,
	// 	out, &Product::name_);

	// std::cout << '\n';
	// EXPECT_EQ(prods.size(), 3);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
TEST(RangesTest, test_count_if)
{
	const std::vector<Product> prods{
		{ "box", 10.0 }, {"tv", 100.0}, {"none", -1.0},
		{ "car", 1000.0 }, {"toy", 40.0}, {"none", 0.0}
	};

	// standard version:
	auto res = std::count_if(begin(prods), end(prods), [](const Product& p) {
		return p.name_.starts_with("no");
		});
	std::cout << "std::count_if: " << res << '\n';

	EXPECT_EQ(res, 2);
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
TEST(RangesTest, test_count_if_ranges)
{
	const std::vector<Product> prods{
		{ "box", 10.0 }, {"tv", 100.0}, {"none", -1.0},
		{ "car", 1000.0 }, {"toy", 40.0}, {"none", 0.0}
	};

	// ranges version:
	auto res = std::ranges::count_if(prods, [](const Product& p) {
		return p.name_.starts_with("no");
		});
	std::cout << "std::ranges::count_if: " << res << '\n';
	EXPECT_EQ(res, 2);

	res = std::ranges::count(prods, std::string{ "none" }, &Product::name_);
	std::cout << "std::ranges::count: " << res << '\n';

	EXPECT_EQ(res, 2);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
TEST(RangesTest, test_find_if)
{
	const std::vector<Product> prods{
		{ "box", 10.0 }, {"tv", 100.0}, {"rocket", 10000.0},
		{ "car", 1000.0 }, {"toy", 40.0}, {"none", 0.0}
	};

	// standard version:
	auto it = std::find_if(begin(prods), end(prods), [](const Product& p) {
		return p.name_.starts_with("ro");
		});
	EXPECT_NE(it, end(prods));

	if (it != end(prods))
		std::cout << "std::find_if: " << it->name_ << '\n';
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
TEST(RangesTest, test_find_if_ranges)
{
	const std::vector<Product> prods{
		{ "box", 10.0 }, {"tv", 100.0}, {"rocket", 10000.0},
		{ "car", 1000.0 }, {"toy", 40.0}, {"none", 0.0}
	};

	// standard version:
	auto it = std::ranges::find_if(prods, [](const Product& p) {
		return p.name_.starts_with("ro");
		});
	EXPECT_NE(it, end(prods));

	if (it != end(prods))
		std::cout << "std::find_if: " << it->name_ << '\n';
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
TEST(RangesTest, test_find_first_of)
{
	const std::vector<Product> prods{
		{ "box", 10.0 }, {"default", 0.0 }, {"tv", 100.0}, {"rocket", 10000.0},
		{ "car", 1000.0 }, {"toy", 40.0}, {"none", 0.0 }, { "ball", 40.0 }
	};

	const std::vector<Product> invalids{
		{"default", 0.0 }, {"none", 0.0 }
	};

	auto cmpNames = [](const Product& a, const Product& b) {
		return a.name_ == b.name_;
	};

	// standard version:
	auto it = std::find_first_of(begin(prods), end(prods),
		begin(invalids), end(invalids), cmpNames);
	EXPECT_NE(it, end(prods));
	if (it != end(prods)) {
		std::cout << "std::find_first_of: " << it->name_ << " at: "
			<< std::distance(begin(prods), it) << '\n';

	auto it2 = std::find_first_of(std::next(it), end(prods),
		begin(invalids), end(invalids), cmpNames);
	EXPECT_NE(it2, end(prods));
	if (it2 != end(prods))
		std::cout << "std::find_first_of: " << it2->name_ << " at: "
			<< std::distance(begin(prods), it2) << '\n';
	}
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
	// if (it != end(prods)) {
	// 	const auto pos = std::distance(begin(prods), it);
	// 	std::cout << "std::ranges::find_first_of: " << it->name_
	// 		<< " at: " << pos << '\n';

	// 	auto it2 = std::ranges::find_first_of(prods | std::views::drop(pos + 1), arrInvalids,
	// 		std::ranges::equal_to{}, &Product::name_);
	// 	EXPECT_NE(it2, prods.end());
	// 	if (it2 != end(prods))
	// 		std::cout << "std::ranges::find_first_of: " << it2->name_
	// 		<< " at: " << std::distance(begin(prods), it2) << '\n';
	// }
}

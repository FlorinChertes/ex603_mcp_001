
#include <test_type_func.h>

#include <type_func.h>

#include <functional>
#include <cmath>
#include <array>
#include <list>
#include <queue>
#include <map>
#include <vector>
#include <iostream>

#include <string>
#include <algorithm>

#include <stdexcept>
#include <cassert>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_type_func_001()
{
	try 
	{
		auto sum_1 = add(1, 2, 3, 4, 5, 6);
		assert(sum_1 == 21);
		std::cout << "sum_1: " << sum_1 << std::endl;
	}
	catch (std::out_of_range&)
	{
		std::cout << "out_of_range exception" << std::endl;
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_type_func_002()
{
	try
	{
		auto sum_2 = add_to_one(2, 3, 4, 5, 6);
		assert(sum_2 == 21);
		std::cout << "sum_2: " << sum_2 << std::endl;

	}
	catch (std::out_of_range&)
	{
		std::cout << "out_of_range exception" << std::endl;
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_type_func_003()
{
	try
	{
		auto m_1 = min(2, 3, 4, 5, 6);
		std::cout << "min: " << m_1 << std::endl;
	}
	catch (std::out_of_range&)
	{
		std::cout << "out_of_range exception" << std::endl;
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_type_func_004()
{
	try
	{
		auto vnums = std::vector<int>{ 0, 2, -3, 5, -1, 6, 8, -4, 9 };

		auto r = funclib::mapf(
			[](int const i) { return std::abs(i); },
			vnums);

		std::copy(r.begin(), r.end(),						//source
			std::ostream_iterator<int>(std::cout, ", "));	//destination
			std::cout << std::endl;
	}
	catch (std::out_of_range&)
	{
		std::cout << "out_of_range exception" << std::endl;
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_type_func_005()
{
	try
	{
		std::map<char, int> m_char_num{ { 'a', 0 },
								   { 'b', 2 },
						           { 'c', -3 },
								   { 'd', 5 },
						           { 'e', -1 },
						           { 'f', 6 },
								   { 'g', 8 },
						           { 'h', -4 },
						           { 'i', 9 }

		};

		auto r = funclib::mapf(
			[](const std::map<char, int>::value_type& kvp)
				{ return std::map<char, int>::value_type (
					kvp.first, std::abs(kvp.second)); },
			m_char_num);

		std::for_each(r.begin(), r.end(),
			[](const std::map<char, int>::value_type& elem)
				{ std::cout << elem.first << ": " <<
							   elem.second << ", "; });
		std::cout << std::endl;

	}
	catch (std::out_of_range&)
	{
		std::cout << "out_of_range exception" << std::endl;
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_type_func_006()
{
	try
	{
		std::queue<int> q_num({ 0, 2, -3, 5, -1, 6, 8, -4, 9 });

		auto r = funclib::mapf(
			[](int const i) { return std::abs(i); },
			q_num);

		while (r.empty() == false)
		{
			std::cout << r.front() << ", ";
			r.pop();
		}
		std::cout << std::endl;

	}
	catch (std::out_of_range&)
	{
		std::cout << "out_of_range exception" << std::endl;
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_type_func_007()
{
	try
	{
		auto lnums = std::list<int>{ 1, 2, 3, 4, 5 };

		auto l = funclib::mapf(
			[](int const i) { return i * i; },
			lnums);

		std::copy(l.begin(), l.end(),						//source
			std::ostream_iterator<int>(std::cout, ", "));	//destination
		std::cout << std::endl;
	}
	catch (std::out_of_range&)
	{
		std::cout << "out_of_range exception" << std::endl;
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template<class T = double>
struct fround
{
	typename std::enable_if<
		std::is_floating_point<T>::value, T>::type
		operator()(const T& value) const
	{
		return std::round(value);
	}
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_type_func_008()
{
	try
	{
		auto amounts =
			std::array<double, 5> {10.42, 2.50, 100.0, 23.75, 12.99};

		auto a = funclib::mapf(fround<>(), amounts);

		std::copy(a.cbegin(), a.cend(),							//source
			std::ostream_iterator<double>(std::cout, ", "));	//destination
		std::cout << std::endl;
	}
	catch (std::out_of_range&)
	{
		std::cout << "out_of_range exception" << std::endl;
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_type_func_009()
{
	try
	{
		auto words = std::map<std::string, int> {
			{"one", 1}, {"two", 2}, {"three", 3} };

		auto m = funclib::mapf(
			[](std::pair<std::string, int> const& kvp) {
				return std::make_pair(
					funclib::mapf(toupper, kvp.first),
								  kvp.second);
			},
			words);

		std::for_each(m.begin(), m.end(),
			[](const std::map<std::string, int>::value_type & elem)
			{ std::cout << elem.first << ": " <<
						   elem.second << ", "; });
		std::cout << std::endl;

	}
	catch (std::out_of_range&)
	{
		std::cout << "out_of_range exception" << std::endl;
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_type_func_010()
{
	try
	{
		auto vnums = std::vector<int>{ 0, 2, -3, 5, -1, 6, 8, -4, 9 };

		auto s1 = funclib::foldl(
			[](int const s, int const n) { return s + n; },
			vnums,
			0);
		std::cout << "foldl result s1 = " << s1 << std::endl;

		auto s2 = funclib::foldl(
			std::plus<>(),
			vnums,
			0);
		std::cout << "foldl result s2 = " << s2 << std::endl;

		auto s3 = funclib::foldr(
			[](const int s, const int n) {return s + n; },
			vnums,
			0);
		std::cout << "foldl result s3 = " << s3 << std::endl;

		auto s4 = funclib::foldr(
			std::plus<>(),
			vnums,
			0);
		std::cout << "foldl result s4 = " << s4 << std::endl;


		auto texts =
			std::vector<std::string>{ std::string("hello"),
				std::string(" "),
				std::string("world"),
				std::string("!") };

		auto txt1 = funclib::foldl(
			[](std::string const& s, std::string const& n) {
				return s + n; },
			texts,
			std::string());
		std::cout << "foldl result txt1 = " << txt1 << std::endl;

		auto txt2 = funclib::foldr(
			[](std::string const& s, std::string const& n) {
				return s + n; },
			texts,
			std::string());
		std::cout << "foldr result txt2 = " << txt2 << std::endl;

		auto s5 = funclib::foldl(
			std::plus<>(),
			funclib::mapf(
				[](int const i) {return i * i; },
				funclib::mapf(
					[](int const i) {return std::abs(i); },
					vnums)),
			0);
		std::cout << "foldl result s5 = " << s5 << std::endl;


		auto s6 = func_lib_ex::foldl(
			[](int const s, int const n) { return s + n; },
			0, 2, -3, 5, -1, 6, 8, -4, 9,
			0);
		std::cout << "foldl result s6 = " << s6 << std::endl;

		auto s7 = func_lib_ex::foldl(
			std::plus<>(),
			0, 2, -3, 5, -1, 6, 8, -4, 9,
			0);
		std::cout << "foldl result s7 = " << s7 << std::endl;
	}
	catch (std::out_of_range&)
	{
		std::cout << "out_of_range exception" << std::endl;
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_type_func_011()
{
	std::cout << "*** test 11 ***" << std::endl;

	auto vnums = std::vector<int>{0,1,2,3,4};
	auto ret_val = func_lib::in(vnums, 0);

	std::cout << "Value was found: " <<
		(ret_val ? "True" : "False") << std::endl;

	auto v_words = std::vector<std::string>{
		std::string("one"),
		std::string("two"),
		std::string("three"),
		std::string("four"),
		std::string("five")};

	const std::string  value_one{ "one" };
	ret_val = func_lib::in(v_words, value_one);

	std::cout << "Value was found: " <<
		(ret_val ? "True" : "False") << std::endl;


/*
	// This call is invalid '0' is not a string,
	// the description is long and
	//needs some experiance to understand

	ret_val = func_lib::in(v_words, 0);

	std::cout << "Value was found: " <<
		(ret_val ? "True" : "False") << std::endl;

//*/

/*
	// This call is invalid '0' is not a string,
	// because of using concepts
	// the description is short and clear

	ret_val = func_lib::in_c(v_words, 0);

	std::cout << "Value was found: " <<
		(ret_val ? "True" : "False") << std::endl;

//*/

	ret_val = func_lib::in_c(vnums, 0);
	std::cout << "Value was found: " <<
		(ret_val ? "True" : "False") << std::endl;

	ret_val = func_lib::in_c(v_words, value_one);

	std::cout << "Value was found: " <<
		(ret_val ? "True" : "False") << std::endl;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
constexpr double harmonic_mean(int a, int b)
{
	double ret_val = 0.0;
	ret_val = 2.0 * a * b / (a + b);

	return ret_val;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_type_func_012()
{
	std::cout << "*** test 12  harmonic mean of two numbers at compile time ***" << std::endl;

	constexpr int a = 3;
	constexpr int b = 5;

	constexpr double hm = harmonic_mean(a, b);
	static_assert(hm == 3.75);

	std::cout << "Value computed at compile time: " << hm << std::endl;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_type_func_013()
{
	std::cout << "*** test 13  harmonic mean of two numbers at run time ***" << std::endl;

	int a = 3;
	const int b = 5;

	const double hm = harmonic_mean(a, b);
	assert(hm == 3.75);

	std::cout << "Value computed at run time: " << hm << std::endl;
}

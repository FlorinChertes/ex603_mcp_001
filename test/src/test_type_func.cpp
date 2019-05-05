
#include <test_type_func.h>

#include <type_func.h>

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

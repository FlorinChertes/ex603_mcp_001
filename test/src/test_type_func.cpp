
#include <type_func.h>

#include <stdexcept>

#include <cassert>
#include <iterator> 
#include <algorithm>
#include <vector>
#include <iostream>


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

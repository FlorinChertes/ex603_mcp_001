
#include <test_chrono_func.h>

#include <type_func.h>

#include <chrono>

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
void test_type_func_chrono_012()
{
	auto start = std::chrono::high_resolution_clock::now();

	[](int const count = 300000000) { for (int i = 0; i < count; ++i); }();

	auto diff = std::chrono::high_resolution_clock::now() - start;

	std::cout << std::chrono::duration<double, std::milli>(diff).count() <<
		"ms" << std::endl;

	std::cout << std::chrono::duration<double, std::nano>(diff).count() <<
		"ns" << std::endl;
}

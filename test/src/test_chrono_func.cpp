
#include <test_chrono_func.h>

#include <chrono>
#include <iostream>

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


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_type_func_chrono_013()
{
	std::chrono::hours half_day(12);
	std::chrono::minutes half_hour(30);

	using namespace std::chrono_literals;

	auto half_day_1 = 12h;
	auto half_hour_1 = 30min;

	std::chrono::hours half_day_in_h(12);
	std::chrono::minutes half_day_in_min(half_day_in_h);
	std::cout << half_day_in_h.count() << "h" << std::endl;			//12h
	std::cout << half_day_in_min.count() << "min" << std::endl;		//720min

	auto total_seconds = 12345s;
	auto hours		= std::chrono::duration_cast<std::chrono::hours>(total_seconds);
	auto minutes	= std::chrono::duration_cast<std::chrono::minutes>(total_seconds % 1h);
	auto seconds	= std::chrono::duration_cast<std::chrono::seconds>(total_seconds % 1min);

	std::cout	<< hours.count()	<< ':'
				<< minutes.count()	<< ':'
				<< seconds.count()	<< std::endl; // 3:25:45
}

template <typename T>
void print_clock()
{
	std::cout << "precision: " << (1000000.0 * double(T::period::num)) / (T::period::den) << std::endl;
	std::cout << "steady:    " << T::is_steady << std::endl;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_type_func_chrono_015()
{
	{
		print_clock<std::chrono::system_clock>();
		print_clock<std::chrono::high_resolution_clock>();
		print_clock<std::chrono::steady_clock>();
	}
}

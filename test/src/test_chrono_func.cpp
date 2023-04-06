
#include <test_chrono_func.h>

#include <ctime>
#include <chrono>
#include <functional>

#include <iomanip>
#include <iostream>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_type_func_chrono_012()
{
	std::cout << "*** test chrono 012 ***" << std::endl;

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
	std::cout << "*** test chrono 013 ***" << std::endl;

	[[maybe_unused]] std::chrono::hours half_day(12);
	[[maybe_unused]] std::chrono::minutes half_hour(30);

	using namespace std::chrono_literals;

	[[maybe_unused]] auto half_day_1 = 12h;
	[[maybe_unused]] auto half_hour_1 = 30min;

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

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
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
	std::cout << "*** test chrono 015 ***" << std::endl;

	{
		print_clock<std::chrono::system_clock>();
		print_clock<std::chrono::high_resolution_clock>();
		print_clock<std::chrono::steady_clock>();
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
auto func = [](int const count = 300000000) { for (int i = 0; i < count; ++i); };
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template <typename Time = std::chrono::microseconds,
		  typename Clock = std::chrono::high_resolution_clock>
	struct perf_timer
{
	template <typename F, typename... Args>
	static Time duration(F&& f, Args... args)
	{
		auto start = Clock::now();
		std::invoke( std::forward<F>(f), std::forward<Args>(args)... );
		auto end = Clock::now();

		return std::chrono::duration_cast<Time>(end - start);
	}
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_type_func_chrono_016()
{
	std::cout << "*** test chrono 016 ***" << std::endl;

	auto t = perf_timer<>::duration(func, 100000000);

	std::cout << std::chrono::duration<double, std::milli>(t).count()
		<< "ms" << std::endl;

	std::cout << std::chrono::duration<double, std::nano>(t).count()
		<< "ns" << std::endl;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_type_func_chrono_017()
{
	std::cout << "*** test chrono 017 ***" << std::endl;

	auto now = std::chrono::system_clock::now();
	auto stime = std::chrono::system_clock::to_time_t(now);
	auto ltime = std::localtime(&stime);
	std::cout << std::put_time(ltime, "%c") << '\n';

	std::cout.imbue(std::locale("de_AT.utf8"));
	std::cout << "de_AT: " << std::put_time(ltime, "%c %Z") << '\n';

	std::cout << "de_AT: " << std::put_time(ltime, "%c") << '\n';
}
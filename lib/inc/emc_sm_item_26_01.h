#include <chrono>
#include <set>
#include <string>

#include <iomanip>
#include <sstream>
#include <iostream>

#include <cassert>

void log_and_add(int idx);
void show_names();
void reset_names();

//-----------------------------------------------------------------------------
void log_and_add_impl(int idx, std::true_type);

//-----------------------------------------------------------------------------
template<typename T>
void print_time(std::chrono::time_point<T> time) {
	time_t time_raw = T::to_time_t(time);
	char buffer[32];

#if _MSC_VER
	struct tm time_info;
	const errno_t err = localtime_s(&time_info, &time_raw);
	if (err) {
		std::string msg{"Invalid argument to localtime_s."};
		throw std::runtime_error{ msg };
	}
	strftime(buffer, 32, "%FT%TZ", &time_info);
#else
	struct tm* time_info;

	time_info = localtime(&time_raw);
	strftime(buffer, 32, "%FT%TZ", time_info);
#endif

	// Calculate the micro-seconds-part of time-duration sice-epoch.
	typename T::duration since_epoch = time.time_since_epoch();
	const std::chrono::seconds seconds_since_epoch =
		std::chrono::duration_cast<std::chrono::seconds>(since_epoch);
	since_epoch -= seconds_since_epoch;
	std::chrono::microseconds micro_seconds =
		std::chrono::duration_cast<std::chrono::microseconds>(since_epoch);

	std::string buffer_str(buffer);
	std::stringstream ss;
	ss << buffer_str.substr(0, 19) << "." << std::setw(6) << std::setfill('0') << micro_seconds.count();

	std::string buffer_str_f{ss.str()};
	std::cout << '[' << buffer_str_f << ']';
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
extern std::multiset<std::string> names;

//-----------------------------------------------------------------------------
template<typename T>
void log_and_add(T&& name)
{
	const auto tp_now = std::chrono::system_clock::now();
	print_time(tp_now);
	std::cout << " insert in container using name\n";

	names.emplace(std::forward<T>(name));
}

//-----------------------------------------------------------------------------
template<typename T>
void log_and_add_impl(T&& name, std::false_type)
{
	const auto tp_now = std::chrono::system_clock::now();
	print_time(tp_now);
	std::cout << " insert in container using name, impl\n";

	names.emplace(std::forward<T>(name));
}

//-----------------------------------------------------------------------------
template<typename T>
void log_and_add_ex(T&& name)
{
	log_and_add_impl(
		std::forward<T>(name),
		std::is_integral<std::remove_reference_t<T>>()
	);
}

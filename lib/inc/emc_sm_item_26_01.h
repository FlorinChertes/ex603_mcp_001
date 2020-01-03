#include <chrono>
#include <set>
#include <string>
#include <iostream>
#include <cassert>

void log_and_add(int idx);
void show_names();
void reset_names();

void log_and_add_impl(int idx, std::true_type);

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
extern std::multiset<std::string> names;

//-----------------------------------------------------------------------------
template<typename T>
void log_and_add(T&& name)
{
	auto now = std::chrono::system_clock::now();
	std::cout <<
		now.time_since_epoch().count() <<
		" " <<
		"insert in container" <<
		std::endl;

	names.emplace(std::forward<T>(name));
}

//-----------------------------------------------------------------------------
template<typename T>
void log_and_add_impl(T&& name, std::false_type)
{
	auto now = std::chrono::system_clock::now();
	std::cout <<
		now.time_since_epoch().count() <<
		" " <<
		"insert in container" <<
		std::endl;

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

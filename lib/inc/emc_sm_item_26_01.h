#include <chrono>
#include <set>
#include <string>

#include <array>
#include <iterator>
#include <iostream>
#include <cassert>

void log_and_add(int idx);
void show_names();
void reset_names();

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


#include <emc_sm_item_26_01.h>

#include <iterator>

std::multiset<std::string> names;

//-----------------------------------------------------------------------------
std::string name_from_idx(int idx)
{
	std::string name("Tom");
	if (idx == 1)
	{
		name = "Jerry";
	}

	return name;
}


//-----------------------------------------------------------------------------
void log_and_add(int idx)
{
	const auto tp_now = std::chrono::system_clock::now();
	print_time(tp_now);
	std::cout << " insert in container using index\n";

	names.emplace(name_from_idx(idx));
}

//-----------------------------------------------------------------------------
void log_and_add_impl(int idx, std::true_type)
{
	log_and_add_ex(name_from_idx(idx));
}

//-----------------------------------------------------------------------------
void show_names()
{
	std::copy(names.begin(), names.end(),						//source
		std::ostream_iterator<std::string>(std::cout, ", "));	//destination
	std::cout << std::endl;
}

//-----------------------------------------------------------------------------
void reset_names()
{
	names.clear();
}

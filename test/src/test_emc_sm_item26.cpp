#include <emc_sm_item_26_01.h>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_emc_item_26_01()
{
	std::cout << "*** test func arg 003 EMC Item 27 ***" << std::endl;

	reset_names();

	std::string one_name("Arla");
	log_and_add(one_name);

	log_and_add(std::string("Pasargade"));
	log_and_add("Persepolis");

	int idx = 1;
	log_and_add(idx);

	show_names();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_emc_item_26_02()
{
	std::cout << "*** test func arg 004 EMC Item 27 ***" << std::endl;

	reset_names();

	std::string one_name("Arla");
	log_and_add_ex(one_name);

	log_and_add_ex(std::string("Pasargade"));
	log_and_add_ex("Persepolis");

	int idx = 1;
	log_and_add_ex(idx);

	short idx_1 = 2;
	log_and_add_ex(idx_1);

	show_names();
}

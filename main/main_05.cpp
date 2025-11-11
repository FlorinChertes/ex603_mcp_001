#include "../test/inc/open_edg/test_array.h"

#include <iostream>
#include <string>

int main ([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
	std::cout << "test main 05" << std::endl;

	test_openEDG_010();
	test_openEDG_011();
	test_openEDG_012();

	test_openEDG_I_1_20();
	test_openEDG_I_2_20_queue();
	test_openEDG_I_2_20_priority_queue();
	test_openEDG_I_2_20_stack();

	return 0;
}

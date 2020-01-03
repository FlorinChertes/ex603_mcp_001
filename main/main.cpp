#include <test_func_arg.h>
#include <test_chrono_func.h>
#include <test_type_func.h>

#include <iostream>
#include <string>


int main (int argc, char *argv[])
{
	
	std::cout << "test_main" << std::endl;
	
	test_type_func_001();
	test_type_func_002();

	test_type_func_003();
	test_type_func_004();

	test_type_func_005();
	test_type_func_006();
	test_type_func_007();

	test_type_func_008();
	test_type_func_008();

	test_type_func_009();

	test_type_func_010();

	test_type_func_011();

	test_type_func_012();
	test_type_func_013();

	test_type_func_chrono_012();
	test_type_func_chrono_013();
	test_type_func_chrono_015();

	test_func_arg_001();
	test_func_arg_002();

	test_func_arg_003();
	test_func_arg_004();

	return 0;
}

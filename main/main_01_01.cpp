#include "test_type_erasure.h"
#include "test_ext_polymorph.h"
#include "test_person.h"

#include "test_book_camp.h"
#include "test_strong_type.h"

#include "test_utils.h"
#include "test_shape.h"

#include <test_overloading_tf.h>

#include <test_lambda.h>
#include <test_unique_ptr.h>
#include <test_emc_sm_item26.h>
#include <test_func_arg.h>
#include <test_chrono_func.h>
#include <test_type_func.h>

#include <test_csv_reader.h>
#include <test_polymorphism.h>
#include <test_polymorphism_variant.h>

#include <test_ranges.h>

#include <iostream>
#include <string>


int main (int argc, char *argv[])
{
	std::cout << "test main 01" << std::endl;

	test_type_func_001();
	test_type_func_002();

	test_type_func_003();
	test_type_func_004();

	test_type_func_005();
	test_type_func_005_1();

	test_type_func_006();
	test_type_func_007();
	test_type_func_008();
	test_type_func_009();

	test_type_func_010();

	test_type_func_011();

	test_type_func_012();
	test_type_func_013();

	test_type_func_chrono_012();
	test_type_func_chrono_013();
	test_type_func_chrono_015();
	test_type_func_chrono_016();

	test_type_func_chrono_017();

	test_func_arg_001();
	test_func_arg_002();
	test_func_arg_003();
	test_func_arg_004();

	test_emc_item_26_01();
	test_emc_item_26_02();

	test_unique_ptr_001();
	test_unique_ptr_002();

	test_lambda_001();

	test_overloading_ft_001();

	test_shape_001();
	test_shape_002();
	test_shape_003();
	test_shape_004();

	test_utils_017();
	test_utils_018();
	test_utils_019();
	test_utils_020();
	test_utils_021();
	test_utils_022();

	test_utils_023();

	test_csv_reader_24();

	test_polymorphism_25_ante();
	test_polymorphism_25();

	test_ranges_026();
	test_ranges_027();
	test_ranges_028();
	test_ranges_028_1();
	test_ranges_028_2();

	test_strong_type_029();
	test_strong_type_030();
	test_strong_type_031();

	test_032();
	test_033();
	test_034();
	test_035();
	test_036();
	test_037();
	test_038();
	test_039();

	return 0;
}

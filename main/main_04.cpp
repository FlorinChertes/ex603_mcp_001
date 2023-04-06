#include "test_type_erasure.h"
#include "test_ext_polymorph.h"
#include "test_person.h"


#include <iostream>
#include <string>


int main ([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
	std::cout << "test main 04" << std::endl;

	test_person_032();

	test_ext_polimorph_033();
	test_ext_polimorph_034();

	test_type_erasure_035();
	test_type_erasure_036();
	test_type_erasure_037();

	return 0;
}

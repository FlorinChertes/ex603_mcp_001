#include "test_utils.h"
#include "test_shape.h"

#include <test_read_file.h>

#include <iostream>
#include <string>


int main ([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
	
	std::cout << "test main 02" << std::endl;
	
	std::cout << "*** start test_count_words_29 ***" << std::endl;
	test_create_big_file();

	std::cout << "*** start test_count_words_30 ***" << std::endl;
	test_count_words_from_file_as_string();
    std::cout << "*** start test_count_words_31 ***" << std::endl;
	test_count_words_from_file();

	std::cout << "*** start test_count_words_32 ***" << std::endl;
	test_count_words_from_file_read_in_blocks();

	return 0;
}

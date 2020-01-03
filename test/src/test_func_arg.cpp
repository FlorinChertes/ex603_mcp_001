#include <test_func_arg.h>
#include <arg_func.h>

#include <array>
#include <iterator>
#include <iostream>
#include <cassert>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_func_arg_001()
{
	std::cout << "*** test func arg 001 ***" << std::endl;

	int keyVals[] = {1,2,3,4,5,6};
	int mappedVals[arraySize(keyVals)];

	assert(arraySize(keyVals) == 6);

	std::copy(keyVals, keyVals + arraySize(keyVals), 	//source
		std::ostream_iterator<int>(std::cout, ", "));	//destination
	std::cout << std::endl;

	std::copy(keyVals, keyVals + arraySize(keyVals), 	//source
		mappedVals);									//destination

	std::copy(mappedVals, mappedVals + arraySize(mappedVals), 	//source
		std::ostream_iterator<int>(std::cout, ", "));			//destination
	std::cout << std::endl;

	std::cout << "size of array is: " << arraySize(keyVals) << std::endl;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_func_arg_002()
{
	std::cout << "*** test func arg 002 ***" << std::endl;

	const int the_answer = 43;
	auto x = the_answer;
	auto y = &the_answer;

	std::cout << "the asswer is: " << *y << std::endl;

	std::cout << "the x type is: " << typeid(x).name() << std::endl;
	std::cout << "the y type is: " << typeid(y).name() << std::endl;
}


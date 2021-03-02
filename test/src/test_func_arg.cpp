#include <test_func_arg.h>
#include <arg_func.h>

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <string_view>

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

std::string generateQuoteTransform(std::string_view quote) //take the input as string_view to avoid a copy.
{
	const char* L = " like "; //that extra space at the frong has a prupose

	std::istringstream ss(quote.data()); // create a stringstream from the quote
	std::ostringstream out;

	std::transform(std::istream_iterator<std::string>(ss),
		std::istream_iterator<std::string>(),
		std::ostream_iterator<std::string>(out, ""),
		[L](auto str) { return L + str; }
	);
	return std::move(out).str();    // leave it like so, then you'll gain a benefit in c++20
}


void test_func_arg_003()
{
	std::cout << "*** test func arg 003 ***" << std::endl;
	std::cout << generateQuoteTransform("I am the most handsome in the world") << "\n";
}


void draw_controls(std::vector<std::unique_ptr<controlbase>>& v)
{
	for (auto& c : v)
	{
		c->draw();
	}
}

void test_func_arg_004()
{
	std::cout << "*** test func arg 004 ***" << std::endl;

	std::vector<std::unique_ptr<controlbase>> v;

	v.emplace_back(std::make_unique<button>());
	v.emplace_back(std::make_unique<checkbox>());

	draw_controls(v);
}

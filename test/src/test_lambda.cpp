#include<algorithm>
#include<vector>
#include<iostream>
#include<cassert>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void test_lambda_001()
{
	struct {
		void operator()(int x) const {
			std::cout << x << '\n';
		}
	} someInstance;

	const std::vector<int> v{ 1, 2, 3, };
	
	std::for_each(v.cbegin(), v.cend(), someInstance);
	
	std::for_each(v.cbegin(), v.cend(), 
		[](int x) {
			std::cout << x << '\n';
		}

	);
}
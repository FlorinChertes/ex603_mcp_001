#include <iostream>
#include <cassert>
#include <memory>

struct Widget {
	explicit Widget(int val) : val_{val} {}

	int get() const {return val_;}

	int val_ {};
};

void sink(std::unique_ptr<Widget> uniqPtr) {
	std::cout << uniqPtr->get() << '\n';
	// do something with uniqPtr
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_unique_ptr_001()
{
	std::cout << "*** test unique_ptr 001 ***" << std::endl;

	std::unique_ptr<int> p1 = std::make_unique<int>(42);
	std::unique_ptr<int> p2;

	std::cout << "p1 = " << p1.get() << std::endl;
	std::cout << "p2 = " << p2.get() << std::endl;

	auto uniqPtr = std::make_unique<Widget>(1998);
	sink(std::move(uniqPtr));      // there is no copy ctor only copy move ctor
	
}

class A
{
public:
	A(int val)
		: val_(val)
	{}

	int GetVal() { return val_; }

private:
	int val_;
};

void myFunc_01(std::unique_ptr<A> arg)
{
	std::cout << arg->GetVal() << std::endl;
}

void myFunc_02(std::unique_ptr<A> arg)
{
	std::unique_ptr<A> ptr(std::move(arg));
	assert(arg == nullptr);

	std::cout << ptr->GetVal() << std::endl;
}

std::unique_ptr<A> myFunc_03()
{
	std::unique_ptr<A> ptr = std::make_unique<A>(7890);
	assert(ptr != nullptr);

	return ptr;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_unique_ptr_002()
{
	std::cout << "*** test unique_ptr 002 ***" << std::endl;

	std::unique_ptr<A> ptr = std::make_unique<A>(1234);
	myFunc_01(std::move(ptr));
	assert(ptr == nullptr);

	std::unique_ptr<A> ptr_1 = std::make_unique<A>(5678);
	myFunc_02(std::move(ptr_1));
	assert(ptr_1 == nullptr);

	std::unique_ptr<A> ptr_2 = myFunc_03();
	assert(ptr_2 != nullptr);
	std::cout << ptr_2->GetVal() << std::endl;
}

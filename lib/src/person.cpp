//---- <Person.cpp> ----------------

#include "person.h"
#include <string>

struct Person::Impl
{
	// ...
	std::string forename;
	std::string surname;
	std::string address;
	std::string city;
	std::string country;
	std::string zip;
	int year_of_birth;
	// ... Potentially many more data members
};

Person::Person()  
   : pimpl_{ std::make_unique<Impl>() }
{}

Person::~Person() = default;  

Person::Person(const std::string& surname)
	: pimpl_{ std::make_unique<Impl>() }
{
	pimpl_->surname = surname;
}

Person::Person( Person const& other )  
   : pimpl_{ std::make_unique<Impl>(*other.pimpl_) }
{}

Person& Person::operator=( Person const& other )
{
   *pimpl_ = *other.pimpl_;
   return *this;
}

Person::Person( Person&& other )  
   : pimpl_{ std::make_unique<Impl>(std::move(*other.pimpl_)) }
{}

Person& Person::operator=( Person&& other )  
{
   *pimpl_ = std::move(*other.pimpl_);
   return *this;
}

int Person::year_of_birth() const  
{
   return pimpl_->year_of_birth;
}

const std::string& Person::surname() const
{
	return pimpl_->surname;
}

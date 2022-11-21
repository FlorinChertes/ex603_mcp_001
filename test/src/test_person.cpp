//---- <Person.cpp> ----------------

#include "../../lib/inc/person.h"
#include <iostream>

void test_person_032()
{
	std::cout << "*** test person 32 ***" << std::endl;

	Person p_01("Oscar");
	std::cout << "surname: " << p_01.surname() << std::endl;

	Person p_02(std::move(p_01));
	std::cout << "surname: " << p_01.surname() << std::endl;
	std::cout << "surname: " << p_02.surname() << std::endl;

}



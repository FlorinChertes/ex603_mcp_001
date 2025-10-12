#include "../../lib/inc/open_edg/oedg_array.h"

#include <iostream>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_openEDG_010()
{
    std::cout << "\n*** test openEDG 010 ***" << std::endl;
    const int n{ 3 };
    Array array(n);

    try {
        array[0] = 10;
        array[1] = 20;
        array[2] = 30;
        for (int i = 0; i < n; ++i)
        {
            std::cout << "array[" << i << "] = " << array[i] << '\n';
        }
    }
    catch (std::exception& e)
    {
        std::cout << "Exception caught: " << e.what() << '\n';
    }

    try
    {
        array[n] = 40; // this should throw an exception
    }
    catch (std::exception& e)
    {
        std::cout << "Exception caught: " << e.what() << '\n';
	}

    try
    {
        array.add(40);
        for (int i = 0; i < n + 1; ++i)
        {
            std::cout << "array[" << i << "] = " << array[i] << '\n';
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "Exception caught: " << e.what() << '\n';
    }

    try
    {
		array.delItem(0);
        for (int i = 0; i < n; ++i)
        {
            std::cout << "array[" << i << "] = " << array[i] << '\n';
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "Exception caught: " << e.what() << '\n';
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_openEDG_011()
{
    std::cout << "\n*** test openEDG 011 ***" << std::endl;

    Array a(10);
    for (unsigned i = 0; i < a.getSize(); ++i)
    {
        a[i] = i;
    }
    for (unsigned i = 0; i < a.getSize(); ++i)
    {
        std::cout << a[i] << " ";
    }
    std::cout << "\n";

}

#include <vector>
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_openEDG_012()
{
    std::cout << "\n*** test openEDG 012 ***" << std::endl;

    std::vector <float> v1(5);
	v1[0] = 1.1f;
	v1[1] = v1[0] + 5.0f;
    v1[2] = v1[0] + 0.5f;
    v1[3] = v1[0] + 2.3f;
    v1[4] = v1[0] + 4.7f;

    for (unsigned i = 0; i < v1.size(); ++i)
    {
        std::cout << v1[i] << " ";
	}

}
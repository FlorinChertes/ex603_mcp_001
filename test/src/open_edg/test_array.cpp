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

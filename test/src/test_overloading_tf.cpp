#include <string>
#include <iostream>

template<int rows>
class Ret_val
{

};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template<int rows>
int func_a()
{
    static int ret = 153;

    std::cout << "func_a ret int rows:" << rows << std::endl;

    return ret;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template<int rows>
 Ret_val<rows> func_b()
{
    static Ret_val<rows> ret;

    std::cout << "func_a ret int rows:" << rows << std::endl;

    return ret;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/*
template<int rows>
std::string func_a()
{
    static int ret = 153;

    std::cout << "func_a ret std::string rows:" << rows << std::endl;

    return ret;
}
*/

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_overloading_ft_001()
{
    std::cout << "*** test overloading function template 001 ***" << std::endl;

    int ret_val = func_a<2>();
    std::cout << "ret val:" << ret_val << std::endl;

    [[maybe_unused]] Ret_val<2> ret_val_1 = func_b<2>();
    std::cout << "ret val 1:" << "Ret_val<2>" << std::endl;

    [[maybe_unused]] Ret_val<3> ret_val_2 = func_b<3>();
    std::cout << "ret val 2:" << "Ret_val<3>" << std::endl;

}


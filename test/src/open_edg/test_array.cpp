#include "../../lib/inc/open_edg/oedg_array.h"

#include <deque>
#include <stack>

#include <list>
#include <vector>
#include <queue>

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
    std::cout << "\n";

    {
        int t[] = { 5, 4, 3, 1, 2 };
        std::vector<int> v1(t, t + 5);
        std::vector<int> v2{ 1,2,3,5,4 };
        std::vector<int> sums(v1.size());
        int x{3};

        for (unsigned i = 0; i < v1.size(); ++i)
            sums[i] = x + v1[i] + v2[i];
        std::cout << "sums: ";

        for (const auto& sum : sums)
            std::cout << sum << " ";
        std::cout << std::endl;
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template <typename T>
std::ostream& print(T const& start
    , T const& end)
{
    for (T i = start; i != end; ++i)
    {
        std::cout << *i << " ";
    }
    return std::cout;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_openEDG_I_1_20()
{
    std::cout << "\n*** test openEDG I_1_20 ***" << std::endl;

    //1/20
    int tab[] = { 1,2,3,4,5,6,7,8,9,10 };
    std::list<int> l1(tab, tab + 10);

    std::list<int>::iterator it = std::next(l1.begin(), 3);
    std::list<int>::const_iterator it_start = it;
    std::advance(it, 1);
    l1.erase(it_start, it);

    print(l1.begin(), l1.end());
    l1.clear();
    std::cout << "\nliste size: " << l1.size() << std::endl;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_openEDG_I_2_20_queue()
{
    std::cout << "\n*** test openEDG I_2_20_queue ***" << std::endl;

    std::queue<int> q_1;
    q_1.push(1);
    q_1.push(2);
    q_1.pop();

    std::deque<int> d_2;
    std::queue<int> q_2{ d_2 };
    q_2.push(1);
    q_2.push(2);
    q_2.pop();


    std::list<int> l_4;
    std::queue<int, std::list<int>> q_4(l_4);
    q_4.push(1);
    q_4.push(2);
    q_4.pop();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_openEDG_I_2_20_priority_queue()
{
    std::cout << "\n*** test openEDG I_2_20_priority_queue ***" << std::endl;

    std::priority_queue<int> pq_1{};
    pq_1.push(1);
    pq_1.push(2);
    pq_1.pop();


    std::deque<int> d_2{};
    std::priority_queue<int, std::deque<int>> pq_2{ std::less<int>(), d_2 };
    pq_2.push(1);
    pq_2.push(2);
    pq_2.pop();

    std::deque<int> d_3{};
    std::priority_queue<int, std::deque<int>> pq_3{ d_3.begin(), d_3.end() };
    pq_3.push(1);
    pq_3.push(2);
    pq_3.pop();

    std::vector<int> v_4;
    std::priority_queue<int, std::vector<int>> pq_4(std::less<int>(), v_4);
    pq_4.push(1);
    pq_4.push(2);
    pq_4.pop();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_openEDG_I_2_20_stack()
{
    std::cout << "\n*** test openEDG I_2_20_stack ***" << std::endl;

    std::stack<int> s_1;
    s_1.push(1);
    s_1.push(2);
    s_1.pop();

    std::deque<int> d_2;
    std::stack<int> s_2{ d_2 };
    s_2.push(1);
    s_2.push(2);
    s_2.pop();


    std::vector<int> v_3;
    std::stack<int, std::vector<int>> s_3(v_3);
    s_3.push(1);
    s_3.push(2);
    s_3.pop();

    std::list<int> l_4;
    std::stack<int, std::list<int>> s_4(l_4);
    s_4.push(1);
    s_4.push(2);
    s_4.pop();
}

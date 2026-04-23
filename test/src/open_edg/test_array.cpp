#include "../../lib/inc/open_edg/oedg_array.h"

#include <deque>
#include <stack>

#include <list>
#include <vector>
#include <queue>

#include <functional>
#include <algorithm>
#include <string>

#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>


template <typename T>
std::ostream& print(
    T const& start
    , T const& end)
{
    for (T i = start; i != end; ++i)
    {
        std::cout << *i << " ";
    }
    return std::cout;
}

class A
{
public:
    int a_;

    A(int a) : a_(a) { std::cout << "A constructor" << std::endl; }
    ~A() { std::cout << "A dtor" << std::endl; }
};

std::ostream& operator<< (std::ostream& c, const A& o)
{
    c << o.a_;
    return c;
}

void fill(const int table[], unsigned size, std::vector<A*>& v)
{
    for (unsigned i = 0; i < size; ++i)
    {
        std::cout << "i: " << i << " " << std::endl;
        v.push_back(new A(table[i]));
    }
}

void fill_(const int table[], unsigned size, std::vector<A>& v)
{
    for (unsigned i = 0; i < size; ++i)
    {
        std::cout << "i: " << i << " " << std::endl;
        v.push_back(A(table[i]));
    }
}

void del(A* p)
{
    delete p;
}

template <typename T>
std::ostream& print_(
    T& start
    , T& end)
{
    for (; start != end; ++start)
    {
        std::cout << *start << " ";
    }
    return std::cout;
}


int tab[] = { 1,2,3,4,5,6,7,8,9,10 };

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

    //---
	// does not compile using C++11
    std::deque<int> d_3{};
    std::queue<int> q_3{ d_3.begin(), d_3.end() };
    q_3.push(1);
    q_3.push(2);
    q_3.pop();

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


    //std::list<int> l_4;
    //std::priority_queue<int, std::list<int>> pq_5(std::less<int>(), l_4);
    //pq_5.push(1);
    //pq_5.push(2);
    //pq_5.pop();
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

    //---
    std::deque<int> d_5;
    std::stack<int> s_5{ d_5.begin(), d_5.end() };
    s_5.push(1);
    s_5.push(2);
    s_5.pop();

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_openEDG_II_09_20()
{
    std::cout << "\n*** test openEDG II_09_20 ***" << std::endl;
    int tab[] = { 1,2,3,4,5,6,7,8,9,10 };

    std::deque<int> d1(tab, tab + 10);
    std::deque<int> d2;
    std::deque<int>::iterator it;

    for (it = d1.begin(); it != d1.end(); ++it)
    {
        d2.push_back(d1[d1.end() - it - 1]);
    }

	//print_(d2.rbegin(), d2.rend()) << std::endl;      // LINE II
    print(d2.rbegin(), d2.rend()) << std::endl;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_openEDG_II_20_20()
{
    std::cout << "\n*** test openEDG II_20_20 ***" << std::endl;
    int tab[] = { 1,2,3,4,5,6,7,8,9,10 };

    std::vector<int> v1(tab, tab + 10);
    std::vector<int> v2(v1.size(), 0);
    try
    {
        for (unsigned i = 0; i <= v1.size(); ++i)
        {
            //int tmp = v1[i];				//LINE I
            //v1[i] = v1.at(v1.size() - 1);	//LINE II
            //v1.at(i) = tmp;					//LINE III
            //std::cout << v1[i] << " ";
        }
    }
    catch (...)
    {
        std::cout << "Exception!" << std::endl;
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_openEDG_III_15_20()
{
    std::cout << "\n*** test openEDG II_20_20 ***" << std::endl;

    std::cout << "start_\n";

    std::vector<A*> v1{};
    fill(tab, 10, v1);
    std::cout << "10 As created" << std::endl;

    std::vector<A*>::iterator it{};
    std::list<A> l1{};
    for (it = v1.begin(); it != v1.end(); ++it)
    {
        l1.push_front(**it);
    }
    print(l1.begin(), l1.end()) << std::endl;
	std::for_each(v1.begin(), v1.end(), del);

    std::cout << "\nend\n";
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_openEDG_III_15_20bis()
{
    std::cout << "\n*** test openEDG II_20_20bis ***" << std::endl;

    std::cout << "start_\n";

    std::vector<A> v2{};
    fill_(tab, 10, v2);
    std::cout << "10 As created" << std::endl;

    std::vector<A>::iterator it{};
    std::list<A> l1{};

    for(it = v2.begin(); it != v2.end(); ++it)
    {
        l1.push_front(*it);
    }

    print(l1.begin(), l1.end()) << std::endl;

    std::cout << "\nend\n";
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_openEDG_VIII_8_1_1_23()
{
    std::cout << "\n*** test EDG IX 8.1.1.23 ***" << std::endl;

    char c;
    unsigned char uc;
    bool b;
    int i;
    unsigned int ui;
    long l;
    float f;
    double d;
    char text[100];

    // when readinng car - only first char is read and stored into variable
    std::cout << "Input - char:\n";
    std::cin >> c;
    std::cout << "Value read: " << c << std::endl;

    std::cout << "Input - unsigned char:\n";
    std::cin >> uc;
    std::cout << "Value read: " << uc << std::endl;

    std::cout << "Input - bool:\n";
    std::cin >> b;
    std::cout << "Value read: " << std::boolalpha << b << std::endl;

    std::cout << "Input - int:\n";
    std::cin >> i;
    std::cout << "Value read: " << i << std::endl;

    std::cout << "Input - unsigned int - reading as hex:\n";
    std::cin >> std::hex >> ui;
    std::cout << "Value read: " << std::hex << std::showbase << ui << std::endl;

    std::cout << std::dec << "Input - long:\n";
    std::cin >> std::dec >> l;
    std::cout << "Value read: " << l << std::endl;

    std::cout << "Input - float:\n";
    std::cin >> f;
    std::cout << "Value read: " << std::fixed << f << std::endl;

    std::cout << "Input - double - scientific:\n";
    std::cin >> d;
    std::cout << "Value read: " << std::fixed << d << std::endl;

    std::cout << "Input - text:\n";
    std::cin >> text;
    std::cout << "Value read: " << text << std::endl;

    std::cout << "\nend\n";
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_openEDG_VIII_8_1_1_24()
{
    std::cout << "\n*** test EDG IX 8.1.1.24 ***" << std::endl;

    char c;
    char buff[255];

    std::cout << "Reading single character:\n";
    c = std::cin.get();
    std::cout << "Value read: " << c << std::endl;

    std::cin.get(c);
     std::cout << "Value read: " << c << std::endl << std::endl;

    std::cout << "Something wrong - remember get does not extract delimiting character!\n";
    std::cout << "Reading single character again:\n";
    std::cin.get(c);
    std::cin.get();			// just to skip newline
    std::cout << "Value read: " << c << std::endl << std::endl;

    std::cout << "Reading the whole line:\n";
    std::cin.get(buff, 254);
    c = std::cin.get();
    std::cout << "Value read: " << buff << std::endl;
    std::cout << "NOT read delimeter: " << (int)c << std::endl << std::endl;

    std::cout << "Reading the whole line - with delimeter set to space:\n";
    std::cin.get(buff, 254, ' ');
    c = std::cin.get();
    std::cout << "Value read: " << buff << std::endl;
    std::cout << "NOT read delimeter: " << (int)c << std::endl;

    std::cout << "\nend\n";
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_openEDG_VIII_8_1_1_25()
{
    std::cout << "\n*** test EDG IX 8.1.1.25 ***" << std::endl;

    char buff[255];

    std::cout << "Reading the whole line:\n";
    std::cin.getline(buff, 254);
    std::cout << "Value read: " << buff << std::endl << std::endl;

    std::cout << "Reading the whole line - with delimiter set to space:\n";
    std::cin.getline(buff, 254, ' ');
    std::cout << "Value read: " << buff << std::endl;

    std::cout << "\nend\n";
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_openEDG_VIII_8_1_1_26()
{
    std::cout << "\n*** test EDG IX 8.1.1.26 ***" << std::endl;

    std::string buff;

    std::cout << "Reading the whole line:\n";
    std::getline(std::cin, buff);
    std::cout << "Value read: " << buff << std::endl << std::endl;

    std::cout << "Reading the whole line - with delimiting character set to space:\n";
    std::getline(std::cin, buff, ' ');
    std::cout << "Value read: " << buff << std::endl;

    std::cout << "\nend\n";
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_openEDG_VIII_8_1_1_27()
{
    std::cout << "\n*** test EDG IX 8.1.1.27 ***" << std::endl;

    const std::string introduction{ "My name is " };
    std::string buff;

    //std::getline (std::cin, buff);
    buff = "My name is Sarah";
    //buff = "My name is John";
    //buff = "Myrname ys noname";      

    if (buff.find(introduction) != std::string::npos)
    {
        std::string name{ buff.substr(introduction.length()) };
        std::cout << "Hi, " << name << "!" << std::endl;
    }
    else
    {
        std::cout << "Hi, please introduce yourself." << std::endl;
    }

    std::cout << "\nend\n";
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template <class T>
std::string getStatus(const T& stream, const char* fileName)
{
    std::ostringstream s;

    if (stream.is_open())
    {
        s << "File: " << fileName << " was successfully opened!\n";
    }
    else
    {
        s << "Failed to open file: " << fileName << std::endl;
    }

    return s.str();
}

std::string getFlags(const std::ios& stream)
{
    std::stringstream s;

    s << std::boolalpha <<
        "G:" << stream.good() <<
        " E:" << stream.eof() <<
        " F:" << stream.fail() <<
        " B:" << stream.bad();

    return s.str();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_openEDG_VIII_8_2_1_2()
{
    std::cout << "\n*** test EDG IX 8.2.1.2 ***" << std::endl;

    std::cout << "start\n";

    char badName[] = { "data/badfilename.txt" };
    char goodName[] = { "data/inputfile03.txt" };

    std::cout << "Trying to open not existing file: " << badName << "\n";
    std::ifstream file(badName);
    std::cout <<
        "Status: " << getStatus(file, badName) <<
        "Flags: " << getFlags(file) << std::endl << std::endl;


    std::cout << "Opening existing file for reading - using the same object, but with different file: " <<
        goodName << "\n";
    file.open(goodName, std::ios_base::in);
    std::cout <<
        "Status: " << getStatus(file, goodName) <<
        "Flags: " << getFlags(file) << std::endl << std::endl;

    std::cout << "Reading contents of the file - there is an extra end of line character in the file :\n";
    int v;
    while (file >> v)
    {
        std::cout << v << ": flags: " << getFlags(file) << std::endl;
    }
    std::cout <<
        "Status: " << getFlags(file) << std::endl << std::endl;
    file.close();


    std::cout << "Reading contents of the file - different way:\n";
    file.open(goodName, std::ios_base::in);
    while (file.good())
    {
        file >> v;
        std::cout << v << ": flags: " << getFlags(file) << std::endl;
    }
    file.close();
    std::cout <<
        "Status: " << getFlags(file) << std::endl;
    file.close();

    std::cout << "\nend\n";
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_openEDG_VIII_8_2_1_7()
{
    std::cout << "\n*** test EDG IX 8.2.1.7 ***" << std::endl;

    std::cout << "start\n";

    const std::string goodName{ "data/inputfile04.txt" };

    std::ifstream file{};
    file.open(goodName, std::ios_base::in);
    std::cout <<
        "Status: " << getStatus(file, goodName.c_str()) <<
        "Flags: " << getFlags(file) << std::endl << std::endl;

    std::string input_string{};
    std::getline(file, input_string);
    {
        std::cout << "Read line: " << input_string << std::endl;
    }

    int a{ 4 };
    int b{ 15 };

    std::cout << input_string.substr(a, b - a + 1) << std::endl;

    a = 0, b = 11;
    std::cout << input_string.substr(a, b - a + 1) << std::endl;

    a = 15, b = 18;
    std::cout << input_string.substr(a, b - a + 1) << std::endl;


    std::cout << "\nend\n";
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_openEDG_VIII_8_2_1_9()
{
    std::cout << "\n*** test EDG IX 8.2.1.9 ***" << std::endl;

    std::cout << "start\n";

    char outputFile[] = { "data/outputfile06.txt" };
    int t[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    std::list<int> l(t, t + 10);

    std::cout << "Creating file: " << outputFile <<
        " and opening it for writing: \n";
    std::ofstream file(outputFile);
    std::cout <<
        "Status: " << getStatus(file, outputFile) <<
        "Flags: " << getFlags(file) << std::endl;

    std::list<int>::iterator it{ l.begin() };
    for (; it != l.end(); ++it)
    {
        file << std::setw(3) << *it;
    }
    std::cout <<
        "Flags: " << getFlags(file) << std::endl;
    file.close();

    std::cout << "Opening file: " << outputFile <<
        " to write more data: \n";
    file.open(outputFile, std::ios_base::app);
    std::cout <<
        "Status: " << getStatus(file, outputFile) <<
        "Flags: " << getFlags(file) << std::endl;
    file << std::endl;

    std::list<int>::reverse_iterator it1{ l.rbegin() };
    for (; it1 != l.rend(); ++it1)
    {
        file << std::setw(3) << *it1;
    }
    std::cout << "Flags: " << getFlags(file) << std::endl;
    file.close();
    std::cout << "Flags (after closing): " << getFlags(file) << std::endl;

    std::cout << "\nend\n";
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_openEDG_VIII_8_2_1_13()
{
    std::cout << "\n*** test EDG IX 8.2.1.13 ***" << std::endl;

    std::cout << "start\n";

    char fileName[] = { "data/outputfile07.txt" };
    int t[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    std::list<int> l(t, t + 10);

    std::cout << "Creating file: " << fileName << " and opening it for writing: \n";
    std::fstream file(fileName, std::ios_base::out);
    std::cout <<
        "Status: " << getStatus(file, fileName) <<
        "Flags: " << getFlags(file) << std::endl;

    std::list<int>::iterator it{ l.begin() };
    for (; it != l.end(); ++it)
    {
        file << std::setw(3) << *it;
    }
    file.close();

    std::cout << "Opening file: " << fileName << " for reading: \n";
    file.open(fileName, std::ios_base::in);
    std::cout << "Reading contents of the file:\n";
    int v;
    while (file >> v)
    {
        std::cout << std::setw(3) << v;
    }
    std::cout << std::endl;
    std::cout <<
        "Flags: " << getFlags(file) << std::endl;
    file.close();

    std::cout << "\nend\n";
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template <class T>
bool check_status(const T& stream)
{
    return
        stream.is_open() == true
        || stream.good() == true
        || stream.eof() == false
        || stream.fail() == false
        || stream.bad() == false;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_openEDG_VIII_8_2_1_14()
{
    std::cout << "\n*** test EDG IX 8.2.1.14 ***" << std::endl;

    const char goodName[] = { "data/inputfile14.txt" };
    
    std::ifstream file{ goodName, std::ios_base::in };
    if (check_status(file) == false)
    {
        std::cout << "File " << goodName << " error: couldn't open." << std::endl;
    }

    char output_file_name[] = { "data/outputfile14.txt" };
    std::ofstream ofile{ output_file_name };
    if (check_status(ofile) == false)
    {
        std::cout << "File " << output_file_name << " error: couldn't open." << std::endl;
    }

    std::string v;
    int count{ 1 };
    while (file >> v)
    {
        std::cout << v << std::endl;
        ofile << count << ':' << v << std::endl;
        ++count;
    }
    file.close();
    ofile.close();

    file.open(output_file_name, std::ios_base::in);
    if (check_status(ofile) == false)
    {
        std::cout << "File " << output_file_name << " error: couldn't open." << std::endl;
    }

    while (file >> v)
    {
        std::cout << v << std::endl;
    }
    file.close();

    std::cout << "\nend\n";
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_openEDG_VIII_8_2_1_15()
{
    std::cout << "\n*** test EDG IX 8.2.1.15 ***" << std::endl;

    char goodName[] = { "data/inputfile03.txt" };
    std::fstream file;
    std::cout << "Opening file for reading: " << goodName << "\n";
    file.open(goodName, std::ios_base::in);
    std::cout <<
        "Status: " << getStatus(file, goodName) <<
        "Flags: " << getFlags(file) << std::endl;

    std::cout << "Reading contents of the file - line by line :\n";
    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream s(line);
        int v;
        while (s >> v)
        {
            std::cout << std::left << std::setw(3) << v;
        }
        std::cout << std::endl;
    }
    std::cout <<
        "Status: " << getFlags(file) << std::endl;
    file.close();

    std::cout << "\nend\n";
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_openEDG_15_40()
{
    std::cout << "\n*** test EDG 15_40 ***" << std::endl;

/*
    std::string s;
    std::cin  >> s;
    std::cout << s << ", " << s << ", " << std::endl;
*/
	std::string s;
	std::getline(std::cin, s);
    std::cout << s << ", " << s << ", " << std::endl;

    std::cout << "\nend\n";
}

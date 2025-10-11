#include "../../test/inc/open_edg/test_array.h"


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class Array
{
    int* _array;
    unsigned int _size;

public:
    Array(unsigned size = 0);
    virtual ~Array();

    void add(int value);
    void delItem(unsigned index);

    unsigned int getSize() const;
    int& operator[] (unsigned index);
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
Array::Array(unsigned size) :
    _array(0)
    , _size(size)
{
    if (size > 0)
    {
        _array = new int[this->_size];
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void Array::add(int value)
{
    if (_size == 0)
    {
        _array = new int[1];
    }
    else
    {
        int* tmp = new int[_size + 1];
        for (unsigned i = 0; i < _size; i++)
        {
            tmp[i] = _array[i];
        }
        delete[] _array;
        _array = tmp;
    }
    _array[_size++] = value;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void Array::delItem(unsigned index)
{
    if (_size == 1)
    {
        delete[] _array;
        _array = 0;
    }
    else
    {
        int* tmp = new int[_size - 1];
        for (unsigned i = 0, j = 0; i < _size; i++, j++)
        {
            if (i == index)
            {
                j--;
                continue;

            }
            tmp[j] = _array[i];
        }
        delete[] _array;
        _array = tmp;
    }
    _size--;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
unsigned int Array::getSize() const
{
    return _size;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int& Array::operator [] (unsigned index)
{
    if (index > _size - 1)
    {
        std::out_of_range e{"size-error"};
        throw e;
    }
    return _array[index];
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
Array::~Array()
{
    delete[]_array;
}


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

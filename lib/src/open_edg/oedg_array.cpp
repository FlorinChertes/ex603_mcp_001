#include "../inc/open_edg/oedg_array.h"

#include <iostream>
#include <stdexcept>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
Array::Array(unsigned size) :
      _array(nullptr)
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



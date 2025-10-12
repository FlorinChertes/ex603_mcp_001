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


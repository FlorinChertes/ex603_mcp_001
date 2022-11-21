#include <cstddef>
#include <algorithm>
#include <iostream>
#include <numbers>
#include <string>

#include <iostream>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template <typename T, typename Parameter>
class NamedType
{
public:
    explicit NamedType(T const& value) : value_(value) {}
    explicit NamedType(T&& value) : value_(std::move(value)) {}
    T& get() { return value_; }
    T const& get() const {return value_; }
private:
    T value_;
};

using Width = NamedType<double, struct WidthParameter>;
using Height = NamedType<double, struct HeightTag>;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class Rectangle
{
public:
    Rectangle(Width width, Height height) 
        : width_{ width}
        , height_{ height }
    {}

    const Width& get_width() const { return width_; }
    const Height& get_height() const { return height_; }

private:
    Width width_;
    Height height_;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_strong_type_029()
{
    std::cout << "*** test strong type 029 ***" << std::endl;

    Rectangle rectangle(Width{ 10 }, Height{ 12 });
    std::cout << "width: " << rectangle.get_width().get() << std::endl;
    std::cout << "height: " << rectangle.get_height().get() << std::endl;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
struct Width_type
{
    unsigned long long value;
};
constexpr Width_type operator"" _width_in_meter(unsigned long long n)
{
    return Width_type{ n };
}
constexpr  Width_type operator+ (const Width_type& lhs, const Width_type& rhs)
{
    return Width_type{ lhs.value + rhs.value };
}


struct Height_type
{
    unsigned long long value;
};
constexpr Height_type operator"" _height_in_meter(unsigned long long n)
{
    return Height_type{ n };
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class Rectangle_type
{
public:
    Rectangle_type(Width_type width, Height_type height)
        : width_{ width }
        , height_{ height }
    {}

    const Width_type& get_width() const { return width_; }
    const Height_type& get_height() const { return height_; }

private:
    Width_type width_;
    Height_type height_;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_strong_type_030()
{
    std::cout << "*** test strong type 030 ***" << std::endl;

    Width_type width = 10_width_in_meter;
    std::cout << "width: " << width.value << " in meter" << '\n';

    Height_type height = 12_height_in_meter;
    std::cout << "height: " << height.value << " in meter" << '\n';

    //Rectangle_type rectangle(height, width); // compile error
    Rectangle_type rectangle(width, height);

    std::cout << "width: " << rectangle.get_width().value << std::endl;
    std::cout << "height: " << rectangle.get_height().value << std::endl;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_strong_type_031()
{
    std::cout << "*** test strong type 031 ***" << std::endl;

    Width_type width_1 = 11_width_in_meter;
    std::cout << "width 1: " << width_1.value << " in meter" << '\n';

    Width_type width_2 = 12_width_in_meter;
    std::cout << "width 2: " << width_2.value << " in meter" << '\n';

    const auto width_3 = width_1 + width_2;
    std::cout << "width 3: " << width_3.value << " in meter" << '\n';

    Height_type height = 12_height_in_meter;
    std::cout << "height: " << height.value << " in meter" << '\n';
    //const auto width_4 = width_1 + height; // compile error
}


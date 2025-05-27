
#include "../../lib/inc/bartek.h"

#include <array>
#include <vector>
#include <string>

#include <iostream>


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
struct X {
    X(std::initializer_list<int> list)
        : count{ list.size() }
    { puts("X(init_list)");  }

    X(size_t cnt)
        : count{ cnt }
    { puts("X(cnt)"); }

    X()
    { puts("X()"); }

    size_t count{};
};

void test_bartek_001()
{
    std::cout << "\n*** test bartek 001 ***" << std::endl;

    X x;
    std::cout << "x.count = " << x.count << '\n';

    X y{ 1 };
    std::cout << "y.count = " << y.count << '\n';

    X z{ 1, 2, 3, 4 };
    std::cout << "z.count = " << z.count << '\n';

    X w(3);
    std::cout << "w.count = " << w.count << '\n';
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_bartek_002()
{
    std::cout << "\n*** test bartek 002 ***" << std::endl;

    Product base{ 42, "base product" }; // an initial object
    std::cout << base.name() << " created\n";

    std::cout << "Product other { base };\n";
    Product other{ base };

    std::cout << "Product another( base );\n";
    Product another( base );

    std::cout << "Product oneMore = base;\n";
    Product oneMore = base;

    std::cout << "std::array<Product, 2> = { base, other };\n";
    std::array<Product, 2> arr = { base, other };

    std::cout << "calling foo()\n";
    foo(arr[0]);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
Product createProduct() {
    Product temp{ 101, "from createProduct()" };
    return temp;
}
void test_bartek_003()
{
    std::cout << "\n*** test bartek 003 ***" << std::endl;
    std::cout << "calling createProduct()\n";
    Product created = createProduct();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void test_bartek_004()
{
    std::cout << "\n*** test bartek 004 ***" << std::endl;

    std::vector<Product> prods {};
    prods.emplace_back(101, "car");
    prods.emplace_back(202, "box");
    prods.emplace_back(303, "toy");
    prods.emplace_back(404, "mug");
    prods.emplace_back(505, "pencil");
    prods.emplace_back(506, "book");
    prods.emplace_back(507, "copy-book");
    prods.emplace_back(508, "banana");
    prods.emplace_back(509, "apple");

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_bartek_005()
{
    std::cout << "\n*** test bartek 005 ***" << std::endl;

    Product base{ 42, "base" };
    Product first{ base }; // copy ctor called!
    Product second = first; // copy ctor called!

    Product third{ 100, "third" };
    third = second; // assignment operator called!
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_bartek_006()
{
    std::cout << "\n*** test bartek 006 ***" << std::endl;

    Product base{ 42, "base" };

    Product first{ std::move(base) }; // move ctor called!
    std::cout << "base.name() after move: " << base.name() << '\n';

    Product second = std::move(first); // move ctor called!
    std::cout << "first.name() after move: " << first.name() << '\n';

    Product third{ 100, "third" };
    third = std::move(second); // assignment operator called!
    std::cout << "second.name() after move: " << second.name() << '\n';
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_bartek_007()
{
    std::cout << "\n*** test bartek 007 ***" << std::endl;

    MyType0 t0{};
    std::cout << t0.x << '\n';

    MyType1 t1{};
    std::cout << t1.x << '\n';

    MyType2 t2{};
    std::cout << t2.x << '\n';

    MyType3 t3{};
    std::cout << t3.x << '\n';

    //const MyType0 ct0;   //error!
    const MyType1 ct1;   // fine, emptyctor called
    //const MyType2 ct2;   //error!
    const MyType3 ct3;   // fine

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

WattPower::WattPower(const HorsePower& h)
    : power_{ h.getValue() * ToWattsRatio } {
}
HorsePower::HorsePower(const WattPower& w)
    : power_{ w.getValue() / ToWattsRatio } {
}
std::ostream& operator<<(std::ostream& os, const WattPower& w) {
    os << w.getValue() << "W";
    return os;
}
std::ostream& operator<<(std::ostream& os, const HorsePower& h) {
    os << h.getValue() << "hp";
    return os;
}

void test_bartek_008()
{
    std::cout << "\n*** test bartek 008 ***" << std::endl;

    //HorsePower hp_ = 10.; // not possible, copy initialization
    HorsePower hp{ 10. }; // fine
    WattPower w{ 1. }; // fine
    WattPower watts{ hp }; // fine, performs the proper conversion for us!

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_bartek_009()
{
    std::cout << "\n*** test bartek 009 ***" << std::endl;

    std::vector<std::string> string_v {"one", "two", "three"};
    auto i = string_v.begin();
    auto e = string_v.end();
    for (; i != e; ++i)
    {
       std::cout << "elem: " << *i << '\n';
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

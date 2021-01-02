#include <any>

#include <vector>
#include <string>

#include <iostream>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
struct IShape
{
    virtual void draw() const = 0;
    virtual ~IShape() {}
};

struct Square : IShape
{
    void draw() const override
    {
        std::cout << "draw square" << std::endl;
    }
};

struct LightningBolt : IShape
{
    void draw() const override
    {
        std::cout << "draw lightning bolt" << std::endl;
    }
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class Shape
{
public:
    template<typename ConcreteShape>
    Shape(ConcreteShape&& shape)
        : storage{ std::forward<ConcreteShape>(shape) }
        , getter{ [](std::any& storage) -> IShape&
        {
            return std::any_cast<ConcreteShape&>(storage);
        }
    }
    {}
    
    IShape* operator->() 
    { 
        return &getter(storage); 
    }

private:
    std::any storage;
    IShape& (*getter)(std::any&);
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
Shape createShape()
{
    return Square{};
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_shape_001()
{
    std::cout << std::endl << std::endl << "*** test shape 001 ***" << std::endl;

    auto shape = createShape();
    shape->draw();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
Shape shape_factory(std::string shape_name)
{
    if (shape_name == "square")
    {
        return Square{};
    }
    else if (shape_name == "lightning_bolt")
    {
        return LightningBolt{};
    }
    else
    {
        return Square{};
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void test_shape_002()
{
    std::cout << "*** test shape 002 ***" << std::endl;

    auto shape = shape_factory("lightning_bolt");
    shape->draw();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::vector<Shape> build_shapes()
{
    std::vector<Shape> shapes{
        Square{},
        LightningBolt{}
    };

    return shapes;
}


void test_shape_003()
{
    std::cout << "*** test shape 003 ***" << std::endl;

    std::vector<Shape> shapes{ build_shapes() };
    for (auto& shape : shapes)
    {
        shape->draw();
    }
}

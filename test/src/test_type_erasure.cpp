//---- <test_type_erasure.cpp> ----------------


#include "../../lib/inc/type_erasure.h"


//#include <Circle.h>
//#include <Square.h>
#include "../../lib/inc/geometry.h"

#include <memory>
#include <vector>
#include <iostream>

void test_type_erasure_035()
{
	std::cout << "*** test type erasure 35 ***" << std::endl;

	// Create a circle as one representative of a concrete shape type
	Circle circle{ 3.14 };

	// Create a drawing strategy in the form of a lambda
	auto drawer = [](Circle const& c) { 
		std::cout << "draw circle, radius: " << c.radius() << std::endl;
	};


	// Combine the shape and the drawing strategy in a 'Shape' abstraction
	// This constructor call will instantiate a 'detail::OwningShapeModel' for
	// the given 'Circle' and lambda types
	Shape shape1(circle, drawer);

	// Draw the shape
	draw(shape1);

	// Create a copy of the shape by means of the copy constructor
	//Shape shape2(shape1);

	// Drawing the copy will result in the same output
	//draw(shape2);
}


void test_type_erasure_036()
{
	std::cout << "*** test type erasure 36 ***" << std::endl;

	// Create a Square as one representative of a concrete shape type
	Square square{ 3.14 };

	// Create a drawing strategy in the form of a lambda
	auto drawer = [](Square const& s) {
		std::cout << "draw square, side: " << s.side() << std::endl;
	};


	// Combine the shape and the drawing strategy in a 'Shape' abstraction
	// This constructor call will instantiate a 'detail::OwningShapeModel' for
	// the given 'Circle' and lambda types
	Shape shape1(square, drawer);

	// Draw the shape
	draw(shape1);

	// Create a copy of the shape by means of the copy constructor
	//Shape shape2(shape1);

	// Drawing the copy will result in the same output
	//draw(shape2);
}

void draw_all_shapes(std::vector<Shape> const& shapes)
{
	for (auto const& shape : shapes)
	{
		draw(shape);
	}
}



void test_type_erasure_037()
{
	std::cout << "*** test type erasure 37 start ***" << std::endl;

	// Create a drawing strategy in the form of a lambda
	auto square_drawer = [](Square const& s) {
		std::cout << "draw square, side: " << s.side() << std::endl;
	};

	// Create a drawing strategy in the form of a lambda
	auto circle_drawer = [](Circle const& c) {
		std::cout << "draw circle, radius: " << c.radius() << std::endl;
	};


	using Shapes = std::vector<Shape>;
	Shapes shapes;

	//shapes.emplace_back(Square(3.14), square_drawer);
	//shapes.emplace_back(Circle(3.14), circle_drawer);
	//shapes.emplace_back(Square(2.63), square_drawer);

	draw_all_shapes(shapes);

	std::cout << "*** test type erasure 37 end ***" << std::endl;
}


//---- <test_ext_polymorph.cpp> ----------------

void draw(int i);
#include "../../lib/inc/ext_polymorph.h"

#include "../../lib/inc/ext_polymorph_strategy.h"

//#include <Circle.h>
//#include <Square.h>
#include "../../lib/inc/geometry.h"

#include <memory>
#include <vector>
#include <iostream>

void test_ext_polimorph_033()
{
	std::cout << "*** test external polymorphism 33 ***" << std::endl;

	using Shapes = std::vector<std::unique_ptr<ShapeConcept>>;  

	using CircleModel = ShapeModel<Circle,OpenGLDrawStrategy>;  
	using SquareModel = ShapeModel<Square,OpenGLDrawStrategy>;  

	Shapes shapes{};

	// Creating some shapes, each one
	//   equipped with an OpenGL drawing strategy
	shapes.emplace_back(
	  std::make_unique<CircleModel>(
		 Circle{2.3}, OpenGLDrawStrategy(/*...red...*/) ) );

	shapes.emplace_back(
	  std::make_unique<SquareModel>(
		 Square{1.2}, OpenGLDrawStrategy(/*...green...*/) ) );
	
	shapes.emplace_back(
	  std::make_unique<CircleModel>(
		 Circle{4.1}, OpenGLDrawStrategy(/*...blue...*/) ) );

	// Drawing all shapes
	for( auto const& shape : shapes )
	{
	  shape->draw();
	}
}

void draw(int i)
{
	std::cout << "draw int, valu: " << i << std::endl;
}

void test_ext_polimorph_034()
{
	std::cout << "*** test external polymorphism 34 ***" << std::endl;

	auto shape = std::make_unique<ShapeModel<int>>(42);

	shape->draw();  // Drawing the integer  
}


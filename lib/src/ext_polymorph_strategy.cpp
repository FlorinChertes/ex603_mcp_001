#include "ext_polymorph_strategy.h"

//#include <Circle>
//#include <Square>
#include "../../lib/inc/geometry.h"


#include <iostream>

OpenGLDrawStrategy::OpenGLDrawStrategy()
{
}

void OpenGLDrawStrategy::operator()( Circle const& circle ) const
{
	std::cout << "draw circle, radius: " << circle.radius() <<  std::endl;
}

void OpenGLDrawStrategy::operator()( Square const& square ) const
{
	std::cout << "draw square, side: " << square.side() << std::endl;
}

 

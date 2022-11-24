//---- <OpenGLDrawStrategy.h> ----------------

#pragma once

//#include /* OpenGL graphics library headers */

class Circle;
class Square;

class OpenGLDrawStrategy
{
public:
	explicit OpenGLDrawStrategy( /* Drawing related arguments */ );

	void operator()( Circle const& circle ) const;  
	void operator()( Square const& square ) const;  

private:
   /* Drawing related data members, e.g. colors, textures, ... */
};


#ifndef COLOR_H
#define COLOR_H

class Color
{

public:

	Color() : r(0), g(0), b(0), a(255) {}
	
	Color(float r, float g, float b)
	: r(static_cast<unsigned char>(r * 255.0f)),
	  g(static_cast<unsigned char>(g * 255.0f)),
	  b(static_cast<unsigned char>(b * 255.0f)),
	  a(255) {}

	Color(float r, float g, float b, float a)
	: r(static_cast<unsigned char>(r * 255.0f)),
	  g(static_cast<unsigned char>(g * 255.0f)),
	  b(static_cast<unsigned char>(b * 255.0f)),
	  a(static_cast<unsigned char>(a * 255.0f)) {}
	 
	Color(unsigned char r, unsigned char g, unsigned char b)
	: r(r), g(g), b(b), a(255) {}

	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	: r(r), g(g), b(b), a(a) {}

	Color(unsigned int r, unsigned int g, unsigned int b)
	: r(r), g(g), b(b), a(255) {}

public:

	unsigned char r, g, b, a;

};
	

#endif
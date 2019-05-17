#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Vector2.h"
#include <algorithm>
#include <cmath>

#include <iostream>

template<typename T>
inline T avg(const T& a, const T& b)
{
	return (a+b)/2;
}

/// For integral types only
template<typename T>
class Rectangle
{

public:

	Rectangle(const Vector2<T>& p1, const Vector2<T>& p2)
	{
		minX = std::min(p1.x, p2.x);
		maxX = std::max(p1.x, p2.x);
		
		minY = std::min(p1.y, p2.y);
		maxY = std::max(p1.y, p2.y);
	}

	Rectangle(const T& minX, const T& maxX, const T& minY, const T& maxY)
	: minX(minX), maxX(maxX), minY(minY), maxY(maxY) {
		// std::cout << minX << "-" << maxX << " " << minY << "-" << maxY << std::endl;
	}
	
	bool empty() const
	{
		return minX > maxX or minY > maxY;
	}
	
	Rectangle<T> topLeftRect() const
	{
		return Rectangle<T>(minX, avg(minX,maxX), avg(minY,maxY) + 1, maxY);
	}
	
	Rectangle<T> topRightRect() const
	{
		return Rectangle<T>(avg(minX,maxX) + 1, maxX, avg(minY,maxY) + 1, maxY);
	}
	
	Rectangle<T> bottomLeftRect() const
	{
		return Rectangle<T>(minX, avg(minX,maxX), minY, avg(minY,maxY));
	}

	Rectangle<T> bottomRightRect() const
	{
		return Rectangle<T>(avg(minX,maxX) + 1, maxX, minY, avg(minY,maxY));
	}

	Rectangle<T> leftHalf() const
	{
		return Rectangle<T>(minX, avg(minX,maxX), minY, maxY);
	}

	Rectangle<T> rightHalf() const
	{
		return Rectangle<T>(avg(minX,maxX) + 1, maxX, minY, maxY);
	}
	
	Rectangle<T> topHalf() const
	{
		return Rectangle<T>(minX, maxX, avg(minY,maxY) + 1, maxY);
	}
	
	Rectangle<T> bottomHalf() const
	{
		return Rectangle<T>(minX, maxX, minY, avg(minY,maxY));
	}
	
	Vector2<T> topLeftCorner() const
	{
		return Vector2<T>(minX, maxY);
	}

	Vector2<T> topRightCorner() const
	{
		return Vector2<T>(maxX, maxY);
	}
	
	Vector2<T> bottomLeftCorner() const
	{
		return Vector2<T>(minX, minY);
	}
	
	Vector2<T> bottomRightCorner() const
	{
		return Vector2<T>(maxX, minY);
	}
	
	Vector2<T> leftEnd() const
	{
		return Vector2<T>(minX, minY);
	}
	
	Vector2<T> rightEnd() const
	{
		return Vector2<T>(maxX, maxY);
	}
	
	Vector2<T> topEnd() const
	{
		return Vector2<T>(minX, maxY);
	}
	
	Vector2<T> bottomEnd() const
	{
		return Vector2<T>(maxX, minY);
	}
	
	T width() const
	{
		return maxX - minX + 1;
	}
	
	T height() const
	{
		return maxY - minY + 1;
	}

	
public:

	T minX, maxX;
	T minY, maxY;
	
};

typedef Rectangle<unsigned> IntRectangle;


class FloatRectangle
{

public:

	FloatRectangle(float minX, float maxX, float minY, float maxY)
	: minX(minX), maxX(maxX), minY(minY), maxY(maxY) {}
	
	// FloatRectangle(float centerX, float centerY, float width, float height)
	// : minX(centerX - width/2.0f), maxX(centerX + width/2.0f), minY(centerY - height/2.0f), maxY(centerY + height/2.0f) {}
	
	float width() const
	{
		return maxX - minX;
	}
	
	float height() const
	{
		return maxY - minY;
	}
	
public:

	float minX, maxX;
	float minY, maxY;
	
};




#endif
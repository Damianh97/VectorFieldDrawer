#ifndef VECTOR_2_h
#define VECTOR_2_h

#include <cmath>

template<typename T>
class Vector2
{

public:

	Vector2() {}

	Vector2(const T& x, const T& y)
	: x(x), y(y) {}
	
public:

	T x, y;
	
};

template<typename T>
inline Vector2<T> operator-(const Vector2<T>& lhs, const Vector2<T>& rhs)
{
	return Vector2<T>(lhs.x - rhs.x, lhs.y - rhs.y);
}

template<typename T>
inline Vector2<T> operator/(const Vector2<T>& lhs, float rhs)
{
	return Vector2<T>(lhs.x / rhs, lhs.y / rhs);
}

template<typename T>
inline T cross(const Vector2<T>& lhs, const Vector2<T>& rhs)
{
	return lhs.x * rhs.y - lhs.y * rhs.x;
}

template<typename T>
inline T squaredNorm(const Vector2<T>& v)
{
	return v.x * v.x + v.y * v.y;
}

template<typename T>
inline T norm(const Vector2<T>& v)
{
	return sqrt(squaredNorm(v));
}

template<typename T>
inline void swap(Vector2<T>& lhs, Vector2<T>& rhs)
{
	T temp = lhs.x;
	lhs.x = rhs.x;
	rhs.x = temp;
	
	temp = lhs.y;
	lhs.y = rhs.y;
	rhs.y = temp;
}

typedef Vector2<unsigned int> Vector2ui;
typedef Vector2<float> Vector2f;

#endif
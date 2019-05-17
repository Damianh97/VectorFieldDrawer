#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Triangle.h"
#include "Vector2.h"

class Transform
{

public:

	Transform();

	Transform(const Vector2f& position, const Vector2f& orientation);
	
	Vector2f getOrientation() const;
	
	void setPosition(float x, float y);
	
	void setPosition(const Vector2f& position);
	
	void setOrientation(float u, float v);
	
	void setOrientation(const Vector2f& orientation);
	
	void translate(float x, float y);
	
	void scale(float factor);
	
	Vector2f transformVector(float x, float y) const;
	
	Vector2f transformVector(const Vector2f& p) const;
	
	Triangle transformTriangle(const Triangle& t) const;

private:

	float m_matrix[9];

};


inline Transform::Transform()
{
	m_matrix[0] = 1.0f;
	m_matrix[1] = 0.0f;
	m_matrix[2] = 0.0f;
	m_matrix[3] = 0.0f;
	m_matrix[4] = 1.0f;
	m_matrix[5] = 0.0f;
	m_matrix[6] = 0.0f;
	m_matrix[7] = 0.0f;
	m_matrix[8] = 1.0f;
}

inline Transform::Transform(const Vector2f& position, const Vector2f& orientation)
{
	m_matrix[0] = orientation.y;
	m_matrix[1] = orientation.x;
	m_matrix[2] = position.x;
	m_matrix[3] = - orientation.x;
	m_matrix[4] = orientation.y;
	m_matrix[5] = position.y;
	m_matrix[7] = 0.0f;
	m_matrix[6] = 0.0f;
	m_matrix[8] = 1.0f;
}

inline Vector2f Transform::getOrientation() const
{
	return Vector2f(m_matrix[1], m_matrix[4]);
}

inline void Transform::setPosition(float x, float y)
{
	m_matrix[2] = x;
	m_matrix[5] = y;
}

inline void Transform::setPosition(const Vector2f& position)
{
	setPosition(position.x, position.y);
}

inline void Transform::setOrientation(float u, float v)
{
	m_matrix[0] = v;
	m_matrix[1] = u;
	m_matrix[3] = - u;
	m_matrix[4] = v;
}

inline void Transform::setOrientation(const Vector2f& orientation)
{
	setOrientation(orientation.x, orientation.y);
}

inline void Transform::translate(float x, float y)
{
	m_matrix[2] += x;
	m_matrix[3] += y;
}

inline void Transform::scale(float factor)
{
	m_matrix[0] *= factor;
	m_matrix[1] *= factor;
	m_matrix[3] *= factor;
	m_matrix[4] *= factor;
}

inline Vector2f Transform::transformVector(float x, float y) const
{
	Vector2f res;
	res.x = m_matrix[0] * x + m_matrix[1] * y + m_matrix[2];
	res.y = m_matrix[3] * x + m_matrix[4] * y + m_matrix[5];
	return res;
}

inline Vector2f Transform::transformVector(const Vector2f& p) const
{
	return transformVector(p.x, p.y);
}

inline Triangle Transform::transformTriangle(const Triangle& t) const
{
	return Triangle(transformVector(t.p1), transformVector(t.p2), transformVector(t.p3));
}

#endif
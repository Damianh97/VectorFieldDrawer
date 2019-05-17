#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Vector2.h"
#include "Rectangle.h"
#include <algorithm>
#include <vector>

class Triangle
{

public:

	Triangle() {}

	Triangle(const Vector2f& p1, const Vector2f& p2, const Vector2f& p3)
	: p1(p1), p2(p2), p3(p3) {}
	
	void forceClockwise()
	{
		Vector2f v1 = p2 - p1;
		Vector2f v2 = p3 - p2;
		
		if (cross(v1, v2) > 0)
		{
			swap(p1, p2);
		}
	}
	
	FloatRectangle computeBoundingBox()
	{
		float minX = std::min(std::min(p1.x, p2.x), p3.x);
		float maxX = std::max(std::max(p1.x, p2.x), p3.x);
		float minY = std::min(std::min(p1.y, p2.y), p3.y);
		float maxY = std::max(std::max(p1.y, p2.y), p3.y);
		
		return FloatRectangle(minX, maxX, minY, maxY);
	}
	
	// void getEdges(Line& main, Line& top, Line& bottom)
	// {
		// std::vector<Vector2f> sortedVertices = {p1, p2, p3};
		// std::sort(sortedVertices.begin(), sortedVertices.end(), [](const Vector2f& a, const Vector2f& b) -> bool {return a.y < b.y;});
		
		// main = Line(sortedVertices[0], sortedVertices[2]);
		// top = Line(sortedVertices[1], sortedVertices[2]);
		// bottom = Line(sortedVertices[0], sortedVertices[1]);
	// }

public:

	Vector2f p1, p2, p3;

};

inline Vector2f computeFarthestVertex(const std::vector<Triangle>& triangles)
{
	auto farthestVertex = [](const Triangle& t) {
		float d1 = squaredNorm(t.p1);
		float d2 = squaredNorm(t.p1);
		float d3 = squaredNorm(t.p1);
		
		Vector2f result;
		
		float farthest12;
		if (d1 >= d2)
		{
			farthest12 = d1;
			result = t.p1;
		}
		else
		{
			farthest12 = d2;
			result = t.p2;
		}
		
		if (d3 > farthest12)
		{
			result = t.p3;
		}
		
		return result;
	};
	
	Vector2f result = farthestVertex(triangles[0]);
	float maxSquaredDist = squaredNorm(result);
	
	for (unsigned i = 1; i < triangles.size(); i++)
	{
		Vector2f farthest = farthestVertex(triangles[i]);
		float dist = squaredNorm(farthest);
		
		if (dist > maxSquaredDist)
		{
			maxSquaredDist = dist;
			result = farthest;
		}
	}
	
	return result;
}






#endif




















// inline bool pointInTriangle(const Vector2f& p, const Triangle& t)
// {
	// auto sign = [](const Vector2f& p, const Vector2f& v1, const Vector2f& v2) {
		// return (p.x - v1.x)*(v2.y - v1.y) - (p.y - v1.y)*(v2.x - v1.x);
		// return p.x*(v2.y - v1.y) - p.y*(v2.x - v1.x) - x1*y2 + y1*x2;
	// }
	
	// float sign1 = sign(p, t[0], t[1]);
	// float sign2 = sign(p, t[1], t[2]);
	// float sign3 = sign(p, t[2], t[0]);
	
	// bool onePositive = sign1 > 0 or sign2 > 0 or sign3 > 0;
	// bool oneNegative = sign1 < 0 or sign2 < 0 or sign3 < 0;
	
	// return not (onePositive and oneNegative);
// }

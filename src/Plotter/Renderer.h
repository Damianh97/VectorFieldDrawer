#ifndef RENDERER_H
#define RENDERER_H

#include "FrameBuffer.h"
#include "Rectangle.h"
#include "Triangle.h"
#include <string>
#include <vector>

class PixelSectorTable;
class Transform;

class Renderer
{

public:

	Renderer(unsigned int frameBufferWidth, unsigned int frameBufferHeight, unsigned int superSampling) :
	m_frameBuffer(superSampling * frameBufferWidth, superSampling * frameBufferHeight),
	m_visibleArea(0.0f, 10.0f, 0.0f, 10.0f),
	m_superSampling(superSampling) {}
	
	void setVisibleArea(const FloatRectangle& area)
	{
		m_visibleArea = area;
	}
	
	void clear(const Color& clearColor)
	{
		m_frameBuffer.clear(clearColor);
	}
	
	void draw(const Triangle& triangle, const Color& color, const Transform& transform);
	
	void draw(const std::vector<Triangle>& triangles, const Color& color, const Transform& transform);
	
	void save(const std::string& path)
	{
		FrameBuffer resultBuffer(m_frameBuffer.width() / m_superSampling, m_frameBuffer.height() / m_superSampling);
		blit(resultBuffer, m_frameBuffer);
		resultBuffer.save(path);
	}
	
private:

	Vector2f toScreenCoords(const Vector2f& p) const;

	IntRectangle getBoxBufferArea(const FloatRectangle& box);

	void drawTriangleInArea(const IntRectangle& area, const Color& color, PixelSectorTable& table);

	void drawTriangleInRow(const IntRectangle& row, const Color& color, PixelSectorTable& table);
		
	void drawTriangleInColumn(const IntRectangle& col, const Color& color, PixelSectorTable& table);

private:

	FrameBuffer m_frameBuffer;
	FloatRectangle m_visibleArea;
	unsigned int m_superSampling;
	
};

#endif
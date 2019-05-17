#include "Renderer.h"
#include "Transform.h"

#include <iostream>

using namespace std;

enum PixelSector
{
	INSIDE_TRIANGLE,
	SECTOR_1,
	SECTOR_2,
	SECTOR_3,
	UNKNOWN_SECTOR
};

class PixelSectorTable
{

public:

	PixelSectorTable(const Triangle& triangle, const IntRectangle& rect)
	: m_table(rect.height(), vector<PixelSector>(rect.width(), UNKNOWN_SECTOR)),
	  m_rowOffset(rect.minY),
	  m_colOffset(rect.minX)
	{
		Triangle t = triangle;
		t.forceClockwise();
		
		Vector2f v1 = t.p2 - t.p1;
		m_edge1.dx = v1.x;
		m_edge1.dy = v1.y;
		m_edge1.products = cross(t.p1, t.p2);
		
		Vector2f v2 = t.p3 - t.p2;
		m_edge2.dx = v2.x;
		m_edge2.dy = v2.y;
		m_edge2.products = cross(t.p2, t.p3);
	
		Vector2f v3 = t.p1 - t.p3;
		m_edge3.dx = v3.x;
		m_edge3.dy = v3.y;
		m_edge3.products = cross(t.p3, t.p1);
	}
	
	PixelSector findPixelSector(const PixelPos& pos)
	{
		// Instead of calculating the whole signX and comparing to 0
		// we could compare two of its terms to the remaining term 
		// and save one subtraction/addition operation.
		// Performance gain may be negligible (or not) or compiler 
		// might be already optimizing it.
		
		unsigned tableRow = pos.y - m_rowOffset;
		unsigned tableCol = pos.x - m_colOffset;
		
		if (m_table[tableRow][tableCol] != UNKNOWN_SECTOR)
		{
			return m_table[tableRow][tableCol];
		}
		
		Vector2f fpos(static_cast<float>(pos.x) + 0.5f, static_cast<float>(pos.y) + 0.5f);
		
		float sign1 = m_edge1.dx * fpos.y - m_edge1.dy * fpos.x + m_edge1.products;
		if (sign1 > 0)
		{
			m_table[tableRow][tableCol] = SECTOR_1;
			return SECTOR_1;
		}
		
		float sign2 = m_edge2.dx * fpos.y - m_edge2.dy * fpos.x + m_edge2.products;
		if (sign2 > 0)
		{
			m_table[tableRow][tableCol] = SECTOR_2;
			return SECTOR_2;
		}
	
		float sign3 = m_edge3.dx * fpos.y - m_edge3.dy * fpos.x + m_edge3.products;
		if (sign3 > 0)
		{
			m_table[tableRow][tableCol] = SECTOR_3;
			return SECTOR_3;
		}
		
		m_table[tableRow][tableCol] = INSIDE_TRIANGLE;
		return INSIDE_TRIANGLE;
	}

private:

	struct EdgeData
	{
		float dx;
		float dy;
		float products;
	};

	EdgeData m_edge1;
	EdgeData m_edge2;
	EdgeData m_edge3;

	vector<vector<PixelSector> > m_table;
	unsigned m_rowOffset;
	unsigned m_colOffset;
};


// ======================================================================

void Renderer::draw(const Triangle& triangle, const Color& color, const Transform& transform)
{
	// Convert from local to world coordinates
	Triangle transformed = transform.transformTriangle(triangle);
	
	// Convert directly from world coordinates to screen coordinates
	transformed.p1 = toScreenCoords(transformed.p1);
	transformed.p2 = toScreenCoords(transformed.p2);
	transformed.p3 = toScreenCoords(transformed.p3);
	
	// Compute the triangle's bounding box and get the corresponding 
	// buffer area
	FloatRectangle bbox = transformed.computeBoundingBox();
	IntRectangle mainArea = getBoxBufferArea(bbox);
	
	// Do nothing if area is empty
	if (mainArea.empty())
	{
		return;
	}
	
	// Build a pixel sector table for this area, it will allow us to 
	// query, for each pixel, to which of the four "sectors" it belongs.
	PixelSectorTable table(transformed, mainArea);

	// cout << mainArea.minX << "-" << mainArea.maxX << " " << mainArea.minY << "-" << mainArea.maxY << endl;
	
	// Start drawing
	drawTriangleInArea(mainArea, color, table);
}


void Renderer::draw(const vector<Triangle>& triangles, const Color& color, const Transform& transform)
{
	for (unsigned i = 0; i < triangles.size(); i++)
	{
		draw(triangles[i], color, transform);
	}
}


Vector2f Renderer::toScreenCoords(const Vector2f& p) const
{
	// To view coords
	Vector2f result(p.x - m_visibleArea.minX, p.y - m_visibleArea.minY);
	
	// To normalized coords
	result.x /= m_visibleArea.width();
	result.y /= m_visibleArea.height();
	
	// To screen coords
	result.x *= m_frameBuffer.width();
	result.y *= m_frameBuffer.height();
	
	return result;
}

// Given a FloatRectangle returns the IntRectangle that encompasses 
// all the pixels of the frame buffer inside the FloatRectangle 
// or crossed by one of its edges.
IntRectangle Renderer::getBoxBufferArea(const FloatRectangle& box)
{
	int width = m_frameBuffer.width();
	int height = m_frameBuffer.height();
	
	int minX = static_cast<int>(box.minX);
	int maxX = static_cast<int>(box.maxX);
	int minY = static_cast<int>(box.minY);
	int maxY = static_cast<int>(box.maxY);	
	
	if (minX >= width or maxX < 0 or minY >= height or maxY < 0)
	{
		// Return empty rectangle (in an ugly way).
		return IntRectangle(1,0,1,0);
	}
	else
	{
		if (minX < 0) minX = 0;
		if (maxX >= width) maxX = width - 1;
		if (minY < 0) minY = 0;
		if (maxY >= height) maxY = height - 1;
		
		return IntRectangle(minX, maxX, minY, maxY);
	}
}

void Renderer::drawTriangleInArea(const IntRectangle& area, const Color& color, PixelSectorTable& table)
{
	// for (unsigned y = area.minY; y <= area.maxY; y++)
	// {
		// for (unsigned x = area.minX; x <= area.maxX; x++)
		// {
			// if (table.findPixelSector(PixelPos(x, y)) == INSIDE_TRIANGLE)
			// {
				// m_frameBuffer.setPixelColor(x, y, color);
			// }
		// }
	// }
	
	// return;
	
	if (area.empty()) return;
	
	if (area.width() > 1 and area.height() > 1)
	{
		PixelSector topLeft = table.findPixelSector(area.topLeftCorner());
		PixelSector topRight = table.findPixelSector(area.topRightCorner());
		PixelSector bottomLeft = table.findPixelSector(area.bottomLeftCorner());
		PixelSector bottomRight = table.findPixelSector(area.bottomRightCorner());
		
		if (topLeft == topRight and topRight == bottomLeft and bottomLeft == bottomRight)
		{
			if (topRight == INSIDE_TRIANGLE)
			{
				m_frameBuffer.fill(area, color);
			}
			else
			{
				return;
			}
			
		}
		else
		{
			drawTriangleInArea(area.topLeftRect(), color, table);
			drawTriangleInArea(area.topRightRect(), color, table);
			drawTriangleInArea(area.bottomLeftRect(), color, table);
			drawTriangleInArea(area.bottomRightRect(), color, table);
		}
	}
	
	else if (area.width() > 1)
	{
		drawTriangleInRow(area, color, table);
	}
	
	else if (area.height() > 1)
	{
		drawTriangleInColumn(area, color, table);
	}
	
	else
	{
		if (table.findPixelSector(area.bottomLeftCorner()) == INSIDE_TRIANGLE)
		{
			m_frameBuffer.setPixelColor(area.bottomLeftCorner(), color);
		}
	}
}

void Renderer::drawTriangleInRow(const IntRectangle& row, const Color& color, PixelSectorTable& table)
{
	if (row.empty()) return;
	
	PixelSector left = table.findPixelSector(row.leftEnd());
	PixelSector right = table.findPixelSector(row.rightEnd());
	
	if (left == right)
	{
		if (left == INSIDE_TRIANGLE)
		{
			m_frameBuffer.fill(row, color);
		}
		else
		{
			return;
		}
	}
	
	else
	{
		drawTriangleInRow(row.leftHalf(), color, table);
		drawTriangleInRow(row.rightHalf(), color, table);
	}
}
	
void Renderer::drawTriangleInColumn(const IntRectangle& col, const Color& color, PixelSectorTable& table)
{
	if (col.empty()) return;
	
	PixelSector top = table.findPixelSector(col.topEnd());
	PixelSector bottom = table.findPixelSector(col.bottomEnd());
	
	if (top == bottom)
	{
		if (top == INSIDE_TRIANGLE)
		{
			m_frameBuffer.fill(col, color);
		}
		else
		{
			return;
		}
	}
	
	else
	{
		drawTriangleInColumn(col.topHalf(), color, table);
		drawTriangleInColumn(col.bottomHalf(), color, table);
	}
}
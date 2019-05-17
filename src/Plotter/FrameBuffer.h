#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include "Color.h"
#include "Rectangle.h"

#include <string>
#include <vector>

typedef Vector2ui PixelPos;

class FrameBuffer
{

public:

	FrameBuffer(std::size_t width, std::size_t height);
	
	std::size_t width() const;
	
	std::size_t height() const;
	
	void setPixelColor(unsigned int x, unsigned int y, const Color& color);
	
	void setPixelColor(const PixelPos& pos, const Color& color);
	
	const Color& getPixelColor(unsigned int x, unsigned int y) const;
	
	void clear(const Color& clearColor);
	
	void fill(const IntRectangle& area, const Color& color);
	
	void save(const std::string& path);
	
	friend void blit(FrameBuffer& dst, const FrameBuffer& src);
	
private:

	std::vector<Color> m_buffer;
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_heightWidthMinusWidth;
	
};


inline FrameBuffer::FrameBuffer(std::size_t width, std::size_t height) :
	m_buffer(width * height),
	m_width(width),
	m_height(height),
	m_heightWidthMinusWidth(height * width  - width) {}

inline std::size_t FrameBuffer::width() const
{
	return m_width;
}

inline std::size_t FrameBuffer::height() const
{
	return m_height;
}

inline void FrameBuffer::setPixelColor(unsigned int x, unsigned int y, const Color& color)
{
	m_buffer[m_heightWidthMinusWidth  - y * m_width + x] = color;
}

inline void FrameBuffer::setPixelColor(const PixelPos& pos, const Color& color)
{
	setPixelColor(pos.x, pos.y, color);
}

inline const Color& FrameBuffer::getPixelColor(unsigned int x, unsigned int y) const
{
	return m_buffer[m_heightWidthMinusWidth  - y * m_width + x];
}

inline void FrameBuffer::clear(const Color& clearColor)
{
	for (unsigned i = 0; i < m_buffer.size(); i++)
	{
		m_buffer[i] = clearColor;
	}
}

inline void FrameBuffer::fill(const IntRectangle& area, const Color& color)
{
	for (unsigned y = area.minY; y <= area.maxY; y++)
	{
		for (unsigned x = area.minX; x <= area.maxX; x++)
		{
			setPixelColor(x, y, color);
		}
	}
}

#endif
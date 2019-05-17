#include "FrameBuffer.h"
#include "lodepng.h"

void FrameBuffer::save(const std::string& path)
{
	lodepng_encode32_file(path.c_str(), reinterpret_cast<unsigned char*>(&m_buffer[0]), width(), height());
}

void blit(FrameBuffer& dst, const FrameBuffer& src)
{
	unsigned int numSamples = src.width() / dst.width();
	
	for (unsigned row = 0; row < dst.height(); row++)
	{
		for (unsigned col = 0; col < dst.width(); col++)
		{
			unsigned avgR = 0, avgG = 0, avgB = 0;
			
			for (unsigned i = 0; i < numSamples; i++)
			{
				for (unsigned j = 0; j < numSamples; j++)
				{
					const Color& color = src.m_buffer[(numSamples*row + i)*src.width() + numSamples*col + j];
					avgR += color.r;
					avgG += color.g;
					avgB += color.b;
				}
			}
			
			avgR /= numSamples * numSamples;
			avgG /= numSamples * numSamples;
			avgB /= numSamples * numSamples;
			dst.m_buffer[row * dst.width() + col] = Color(avgR, avgG, avgB);
		}
	}
}
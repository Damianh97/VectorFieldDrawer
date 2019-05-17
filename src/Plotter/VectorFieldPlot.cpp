#include "VectorFieldPlot.h"

#include "Rectangle.h"
#include "Renderer.h"

#include <cstdlib>
// #include <cmath>

VectorFieldPlot::VectorFieldPlot
(
	const std::function<Vector2f(float,float)>& func,
	const FloatRectangle& plottedArea,
	unsigned int numArrows,
	float minArrowSize,
	float maxArrowSize,
	float randomDisplacement
)
{	
	m_arrows.reserve(numArrows * numArrows);
	
	float squareSize = plottedArea.width() / numArrows;
	float halfSquareSize = squareSize / 2.0f;
	
	float minNorm = norm(func(plottedArea.minX + halfSquareSize, plottedArea.minY + halfSquareSize));
	float maxNorm = minNorm;
	
	for (unsigned x = 0; x < numArrows; x++)
	{
		for (unsigned y = 0; y < numArrows; y++)
		{
			Vector2f pos;
			float dispX = ((2.0f/RAND_MAX)*static_cast<float>(rand()) - 1) * randomDisplacement * halfSquareSize;
			float dispY = ((2.0f/RAND_MAX)*static_cast<float>(rand()) - 1) * randomDisplacement * halfSquareSize;
			pos.x = plottedArea.minX + x * squareSize + halfSquareSize + dispX;
			pos.y = plottedArea.minY + y * squareSize + halfSquareSize + dispY;

			Vector2f dir = func(pos.x, pos.y);
			float n = norm(dir);
			
			if (n < minNorm)
			{
				minNorm = n;
			}
			
			if (n > maxNorm)
			{
				maxNorm = n;
			}

			m_arrows.push_back(Transform(pos, dir));
		}
	}
	
	for (unsigned i = 0; i < m_arrows.size(); i++)
	{
		float n = norm(m_arrows[i].getOrientation());
		float factor = ( (maxArrowSize - minArrowSize)/(maxNorm - minNorm) )*(n - minNorm) + minArrowSize;
		factor /= n;
		factor *= halfSquareSize;
		m_arrows[i].scale(factor);
	}
}

void VectorFieldPlot::draw(ArrowModel model, const Color& arrowColor, const Color& backgroundColor, Renderer& renderer)
{
	draw(arrowModels[model], arrowColor, backgroundColor, renderer);
}

void VectorFieldPlot::draw(const std::vector<Triangle>& model, const Color& arrowColor, const Color& backgroundColor, Renderer& renderer)
{
	renderer.clear(backgroundColor);

	for (unsigned i = 0; i < m_arrows.size(); i++)
	{
		renderer.draw(model, arrowColor, m_arrows[i]);
	}
}

const std::vector<Triangle> VectorFieldPlot::arrowModels[NUM_ARROW_MODELS] = 
{
	// AM_DEFAULT
	{
		Triangle(Vector2f( - 0.15f, 0.4f - 1.0f), Vector2f(0.15f, 0.4f - 1.0f), Vector2f(0.0f, 1.0f - 1.0f)),
		Triangle(Vector2f( - 0.025f, 0.4f - 1.0f), Vector2f( - 0.025f, - 1.0f - 1.0f), Vector2f(0.025f, - 1.0f - 1.0f)),
		Triangle(Vector2f(0.025f, - 1.0f - 1.0f), Vector2f(0.025f, 0.4f - 1.0f), Vector2f( - 0.025f, 0.4f - 1.0f))
	}
};

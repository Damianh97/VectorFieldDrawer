#ifndef VECTOR_FIELD_PLOT_H
#define VECTOR_FIELD_PLOT_H

#include "Transform.h"
#include "Triangle.h"
#include "Vector2.h"

#include <functional>
#include <vector>

enum ArrowModel
{
	AM_DEFAULT,
	NUM_ARROW_MODELS
};

class Color;
class FloatRectangle;
class Renderer;

class VectorFieldPlot
{

public:

	VectorFieldPlot(const std::function<Vector2f(float,float)>& func, const FloatRectangle& plottedArea, unsigned int numArrows, float minArrowSize, float maxArrowSize, float randomDisplacement);
	
	VectorFieldPlot(const std::vector<Vector2f>& points, const std::vector<Vector2f>& arrows);
	
	void draw(ArrowModel model, const Color& arrowColor, const Color& backgroundColor, Renderer& renderer);
	
	void draw(const std::vector<Triangle>& model, const Color& arrowColor, const Color& backgroundColor, Renderer& renderer);

private:

	static const std::vector<Triangle> arrowModels[NUM_ARROW_MODELS];

	std::vector<Transform> m_arrows;

};

#endif
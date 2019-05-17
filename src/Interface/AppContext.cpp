#include "AppContext.h"

#include <functional>
	
AppContext::AppContext() :
	m_renderer(1024, 1024, 4),
	m_rendererWidth(1024),
	m_rendererHeight(1024),
	m_superSamplingParam(4),
	m_mustUpdateRenderer(false),
	m_finished(false),
	m_arrowModel(AM_DEFAULT),
	m_numArrows(20),
	m_minSize(0.0f),
	m_maxSize(1.0f),
	m_randomDisplacement(0.0f),
	m_arrowColor(Color(0.0f, 0.0f, 0.0f)),
	m_backgroundColor(Color(1.0f, 1.0f, 1.0f)),
	m_plottedArea(-10.0f, 10.0f, -10.0f, 10.0f),
	m_outputFile("output.png")
{
	m_renderer.setVisibleArea(m_plottedArea);
}

void AppContext::plot()
{
	if (m_mustUpdateRenderer)
	{
		m_renderer = Renderer(m_rendererWidth, m_rendererHeight, m_superSamplingParam);
		m_renderer.setVisibleArea(m_plottedArea);
		m_mustUpdateRenderer = false;
	}
	
	const ScalarFunction& func1 = m_functions.at(m_func1);
	const ScalarFunction& func2 = m_functions.at(m_func2);
	
	auto func = [&func1, &func2](float x, float y) -> Vector2f
	{
		return Vector2f( func1.evaluate({x, y}, ConstantMap()), func2.evaluate({x,y}, ConstantMap()) );
	};
	
	VectorFieldPlot plt(func, m_plottedArea, m_numArrows, m_minSize, m_maxSize, m_randomDisplacement);
	plt.draw(m_arrowModel, m_arrowColor, m_backgroundColor, m_renderer);
	m_renderer.save(m_outputFile);
}


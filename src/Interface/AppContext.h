#ifndef APP_CONTEXT_H
#define APP_CONTEXT_H

#include "../Plotter/Color.h"
#include "../Plotter/Rectangle.h"
#include "../Plotter/Renderer.h"
#include "../Plotter/VectorFieldPlot.h"

#include "ScalarFunction.h"
#include "Token.h"

#include <algorithm>
#include <map>
#include <string>
#include <utility>

class AppContext
{

public:

	AppContext();

	
	bool finished() const;
	
	void setFinishedFlag(bool value);
	
	
	void setResolution(unsigned int width, unsigned int height);
	
	void setSuperSamplingParam(unsigned int param);
	
	
	void defineFunction(const std::string& name, ScalarFunction& function);
	
	bool functionDefined(const std::string& name) const;
	
	const std::map<std::string, ScalarFunction>& functionMap() const;
	
	
	void setArrowModel(ArrowModel model);
	
	void setNumArrows(unsigned int numArrows);
	
	void setMinArrowSize(float size);
	
	void setMaxArrowSize(float size);
	
	void setRandomArrowDisplacement(float disp);
	
	void setArrowColor(const Color& color);
	
	void setBackgroundColor(const Color& color);
	
	void setPlottedArea(float minX, float maxX, float minY, float maxY);
	
	void setFirstFunction(const std::string& name);
	
	void setSecondFunction(const std::string& name);
	
	bool firstFunctionDefined() const;
	
	bool secondFunctionDefined() const;
	
	void setOutputFile(const std::string& path);
	
	void plot();
	
private:

	Renderer m_renderer;
	unsigned int m_rendererWidth;
	unsigned int m_rendererHeight;
	unsigned int m_superSamplingParam;
	bool m_mustUpdateRenderer;
	
	bool m_finished;
	
	std::map<std::string, ScalarFunction> m_functions;
	
	ArrowModel m_arrowModel;
	unsigned int m_numArrows;
	float m_minSize;
	float m_maxSize;
	float m_randomDisplacement;
	Color m_arrowColor;
	Color m_backgroundColor;
	FloatRectangle m_plottedArea;
	std::string m_func1;
	std::string m_func2;
	std::string m_outputFile;

};


inline bool AppContext::finished() const
{
	return m_finished;
}

inline void AppContext::setFinishedFlag(bool value)
{
	m_finished = value;
}

inline void AppContext::setResolution(unsigned int width, unsigned int height)
{
	m_rendererWidth = width;
	m_rendererHeight = height;
	m_mustUpdateRenderer = true;
}

inline void AppContext::setSuperSamplingParam(unsigned int param)
{
	m_superSamplingParam = param;
	m_mustUpdateRenderer = true;
}

inline void AppContext::defineFunction(const std::string& name, ScalarFunction& function)
{
	auto it = m_functions.find(name);
	if (it != m_functions.end())
	{
		m_functions.erase(it);
	}
	
	m_functions.insert(std::make_pair(name, std::move(function)));
}

inline bool AppContext::functionDefined(const std::string& name) const
{
	return m_functions.count(name) == 1;
}

inline const std::map<std::string, ScalarFunction>& AppContext::functionMap() const
{
	return m_functions;
}

inline void AppContext::setArrowModel(ArrowModel model)
{
	m_arrowModel = model;
}

inline void AppContext::setNumArrows(unsigned int numArrows)
{
	m_numArrows = numArrows;
}

inline void AppContext::setMinArrowSize(float size)
{
	m_minSize = size;
}

inline void AppContext::setMaxArrowSize(float size)
{
	m_maxSize = size;
}

inline void AppContext::setRandomArrowDisplacement(float disp)
{
	m_randomDisplacement = disp;
}

inline void AppContext::setArrowColor(const Color& color)
{
	m_arrowColor = color;
}

inline void AppContext::setBackgroundColor(const Color& color)
{
	m_backgroundColor = color;
}

inline void AppContext::setPlottedArea(float minX, float maxX, float minY, float maxY)
{
	m_plottedArea = FloatRectangle(minX, maxX, minY, maxY);
	m_renderer.setVisibleArea(m_plottedArea);
}

inline void AppContext::setFirstFunction(const std::string& name)
{
	m_func1 = name;
}
	
inline void AppContext::setSecondFunction(const std::string& name)
{
	m_func2 = name;
}

inline bool AppContext::firstFunctionDefined() const
{
	return m_func1.size() != 0;
}

inline bool AppContext::secondFunctionDefined() const
{
	return m_func2.size() != 0;
}

inline void AppContext::setOutputFile(const std::string& path)
{
	m_outputFile = path;
}

#endif
#ifndef PARSED_COMMAND_H
#define PARSED_COMMAND_H

#include "MathExpression.h"
#include <string>
#include <unordered_map>
#include <vector>

class CommandParser;

class ParsedCommand
{

friend class CommandParser;

public:
	
	template<typename T>
	const T& get(const std::string& param) const;
	
private:

	ParsedCommand();

	std::unordered_map<std::string, std::string> m_ids;
	std::unordered_map<std::string, std::vector<std::string> > m_idLists;
	std::unordered_map<std::string, int> m_ints;
	std::unordered_map<std::string, int> m_floats;
	std::unordered_map<std::string, MathExpression> m_mathExprs;

};

template<>
const std::string& get<std::string>(const std::string& param) const
{
	return m_ids[param];
}

template<>
const std::vector<std::string>& get<std::vector<std::string> >(const std::string& param) const
{
	return m_idLists[param];
}

template<>
const int& get<int>(const std::string& param) const
{
	return m_ints[param];
}

template<>
const float& get<float>(const std::string& param) const
{
	return m_floats[param];
}

template<>
const MathExpression& get<MathExpression>(const std::string& param) const
{
	return m_mathExprs[param];
}

#endif
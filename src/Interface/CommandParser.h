#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include "MathExpression.h"
#include "ParsedCommand.h"
#include <string>
#include <unordered_map>
#include <vector>

enum ParamType
{
	PT_NONE,
	PT_IDENTIFIER,
	PT_IDENTIFIER_LIST,
	PT_INTEGER,
	PT_FLOAT,
	PT_EXPRESSION
};

struct SyntaxElement
{
	bool isParam;
	std::string str;
	ParamType ptype;
};

class CommandParser
{

public:

	CommandParser(const std::vector<SyntaxElement>& syntax)
	{
		m_syntax = syntax;
	}
	
	ParsedCommand parse(const TokenList& tokenizedCmd)
	{
		ParsedCommand res;
		unsigned pos = 0;
		for (unsigned i = 0; i < m_syntax.size(); i++)
		{
			if (m_syntax[i].isParam)
			{
				switch (m_syntax[i].ptype)
				{
				case PT_IDENTIFIER:
					parsed.m_ids[m_syntax[i].str] = readIdentifier(tokenizedCmd, pos);
					break;
				case PT_IDENTIFIER_LIST:
					parsed.m_idLists[m_syntax[i].str] = readIdentifierList(tokenizedCmd, pos);
					break;
				case PT_INTEGER:
					parsed.m_ints[m_syntax[i].str] = readInt(tokenizedCmd, pos);
					break;
				case PT_FLOAT:
					parsed.m_floats[m_syntax[i].str] = readFloat(tokenizedCmd, pos);
					break;
				case PT_MATH_EXPRESSION:
					parsed.m_mathExprs.insert(std::make_pair(m_syntax[i].str, readMathExpr(tokenizedCmd, pos)));
					break;
				default:
					break;
				}
			}
			else
			{
				if (tokenizedCmd[pos] == m_syntax[i].str)
				{
					pos++;
				}
				else
				{
					throw std::runtime_error("Syntax error");
				}
			}
		}
		
		return res;
	}
	
private:

	std::string readIdentifier(const TokenList& tokenizedCmd, unsigned& pos)
	{
		if (pos < tokenizedCmd.size() and tokenizedCmd[pos].type == TT_IDENTIFIER)
		{
			return tokenizedCmd[pos++].str;
		}
		else
		{
			throw std::runtime_error("Syntax error id");
		}
	}
	
	std::vector<std::string> readIdentifierList(const TokenList& tokenizedCmd, unsigned& pos)
	{
		std::vector<std::string> res;
		
		while (pos < tokenizedCmd.size() and tokenizedCmd[i].type == TT_IDENTIFIER)
		{
			res.push_back(tokenizedCmd[i].str);
			pos++;
		}
		
		return res;
	}
	
	int readInt(const TokenList& tokenizedCmd, unsigned& pos)
	{
		if (pos < tokenizedCmd.size() and tokenizedCmd[pos].type == TT_INTEGER)
		{
			return stoi(tokenizedCmd[pos++].str);
		}
		else if (pos + 1 < tokenizedCmd.size() and tokenizedCmd[pos].type == TT_SUB and tokenizedCmd[pos+1].type == TT_INTEGER)
		{
			unsigned i = pos;
			pos += 2;
			return stoi(tokenizedCmd[i].str + tokenizedCmd[i+1].str);
		}
		else
		{
			throw std::runtime_error("Syntax error int");
		}
	}
			
	float readFloat(const TokenList& tokenizedCmd, unsigned& pos)
	{
		if (pos < tokenizedCmd.size() and (tokenizedCmd[pos].type == TT_INTEGER or tokenizedCmd[pos].type == TT_FLOAT))
		{
			return stoi(tokenizedCmd[pos++].str);
		}
		else if
		(
			pos + 1 < tokenizedCmd.size() and
			tokenizedCmd[pos].type == TT_SUB and
			(tokenizedCmd[pos+1].type == TT_INTEGER or tokenizedCmd[pos+1].type == TT_FLOAT)
		)
		{
			unsigned i = pos;
			pos += 2;
			return stof(tokenizedCmd[i].str + tokenizedCmd[i+1].str);
		}
		else
		{
			throw std::runtime_error("Suntax error float");
		}
	}

	MathExpression readMathExpr(const TokenList& tokenizedCmd, unsigned& pos)
	{
		return Expression(tokenizedCmd, pos);
	}
		
private:

	std::vector<SyntaxElement> m_syntax;

};

#endif
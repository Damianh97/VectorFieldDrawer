#include "Expression.h"

ExpressionParser::ExpressionParser(TokenIterator first, TokenIterator last) :
	m_first(first),
	m_last(last),
	m_parens(last - first, last),
	m_actualPos(first),
{
	//We start finding corresponding closing parenthesis for each opening parenthesis
	std::stack<std::size_t> openParens; //Positions of opening parentheses not yet closed
	
	for (auto it = first; it != last; ++it)
	{
		if (it->type == TT_OPEN_PAREN)
		{
			openParens.push(it - first);
		}
		else if (it->type == TT_CLOSE_PAREN)
		{
			if (openParens.empty())
			{
				m_errorStream << "unexpected ')' found while parsing expression.";
				return;
			}
			else
			{
				marks[openParens.top()] = it;
				openParens.pop();
			}
		}
	}
	
	if (!openParens.empty())
	{
		m_errorStream << "missing expected ')' in expression.";
		return;
	}
	
	if (lastComma != last - first)
	{
		marks[lastComma] = std::make_pair(2, last);
	}
}

ExpressionPtr ExpressionParser::parse()
{
	return parse(m_last);
}

std::string ExpressionParser::errorMsg()
{
	std::string msg = m_errorStream.str();
	std::ostringstream().swap(m_errorStream);
}
	
ExpressionPtr ExpressionParser::parse(TokenIterator limit)
{
	std::vector<ExpressionPtr> exprs;
	exprs.push_back(parseNonBinary(limit));
	
	if (exprs.back() == nullptr) return nullptr;
	
	while (m_actualPos != limit)
	{
		switch (m_actualPos->type)
		{
			case TT_ADD:
			case TT_SUB:
			case TT_MUL:
			case TT_DIV:
			case TT_POW:
				exprs.push_back(ExpressionPtr(new BinaryExpression(m_actualPos->str[0])));
				break;
			default:
				m_errorStream << "missing expected binary operator in expression.";
				return nullptr;
		}
		
		++m_actualPos;
		
		if (m_actualPos == limit)
		{
			m_errorStream << "missing right operand for binary operator.";
			return nullptr;
		}

		exprs.push_back(parseNonBinary(limit));
		
		if (exprs.back() == nullptr) return nullptr;
	}
	
	return makeTree(exprs, 0, exprs.size());
}

ExpressionPtr ExpressionParser::parseNonBinary(TokenIterator limit)
{
	if (m_actualPos == limit)
	{
		m_errorStream << "missing non binary expression.";
		return nullptr;
	}
	
	switch (m_actualPos->type)
	{
		case TT_IDENTIFIER:
		{
			const std::string& name = m_actualPos->str;
			++m_actualPos;
			
			if (m_actualPos != limit and m_actualPos->type == TT_OPEN_PAREN)
			{
				TokenIterator closingParen = m_parens[m_actualPos - m_first];
				++m_actualPos;
				ExpressionPtr expr(new Evaluation);
				expr->name = name;
				parseCommaSeparated(closingParen, name->args);
				++m_actualPos;
				return expr;
			}
			else
			{
				ExpressionPtr expr(new Variable);
				expr->name = name;
				return expr;
			}
		}
		
		case TT_OPEN_PAREN:
		{
			TokenIterator closingParen = m_parens[m_actualPos - m_first];
			++m_actualPos;
			ExpressionPtr expr(parse(closingParen));
			++m_actualPos;
			return expr:
		}
	
		case TT_INTEGER:
		case TT_FLOAT:
		{
			ExpressionPtr expr(new Constant);
			expr->value = std::stof(m_actualPos->str);
			return expr:
		}
		
		case TT_ADD:
			++m_actualPos;
			return parse(limit);
		
		case TT_SUB:
			ExpressionPtr expr(new Negation);
			++m_actualPos;
			expr->operand = parse(limit);
			return expr;
			
		default:
			m_errorStream << "expected non binary expression.";
			return nullptr;
	}			
}

void ExpressionParser::parseCommaSeparated(TokenIterator limit, std::vector<ExpressionPtr>& exprs)
{
	auto it = m_actualPos;
	for (; it != limit; ++it)
	{
		if (it->type == TT_COMMA)
		{
			exprs.push_back(parse(it));
			if (exprs.back() == nullptr) return;
			++m_actualPos;
		}
	}
	
	exprs.push_back(parse(it));
	if (exprs.back() == nullptr) return;
}

ExpressionPtr ExpressionParser::makeTree(const std::vector<ExpressionPtr>& nodes, std::size_t start, std::size_t end)
{
	if (end - start == 1)
	{
		return nodes[start];
	}
	else
	{
		std::size_t mostPrecedent = start + 1;
		unsigned int maxPrecedence = binOpPrecedences[static_cast<const BinaryExpression*>(nodes[mostPrecedent].get())->op];
		
		for (unsigned i = mostPrecedent + 2; i < end; i += 2)
		{
			const BinaryExpression* actual = static_cast<const BinaryExpression*>(nodes[i].get());
			if (binOpPrecedences[actual->op] >= maxPrecedence)
			{
				maxPrecedence = binOpPrecedences[actual->op];
				mostPrecedent = i;
			}
		}
		
		BinaryExpression* root = static_cast<BinaryExpression*>(nodes[mostPrecedent].get());
		root->leftOperand = makeTree(nodes, start, mostPrecedent);
		root->rightOperand = makeTree(nodes, mostPrecedent + 1, end);
		
		return nodes[mostPrecedent];
	}
}
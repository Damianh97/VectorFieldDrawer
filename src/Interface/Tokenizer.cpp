#include "Tokenizer.h"

#include <cctype>
#include <stdexcept>
#include <unordered_set>

/* TODO:
 * - use smart pointers.
 * - fix error reporting.
 */
namespace
{

CharType getCharType(char c)
{
	if (isalpha(c) or c == '_')
	{
		return CT_LETTER_OR_UNDERSCORE;
	}
	else if (isdigit(c))
	{
		return CT_NUMBER;
	}
	else if (c == '.')
	{
		return CT_DOT;
	}
	else if (c == ',')
	{
		return CT_COMMA;
	}
	else if (c == '(')
	{
		return CT_OPEN_PAREN;
	}
	else if (c == ')')
	{
		return CT_CLOSE_PAREN;
	}
	else if (c == '=')
	{
		return CT_EQUAL;
	}
	else if (c == '+')
	{
		return CT_ADD;
	}
	else if (c == '-')
	{
		return CT_SUB;
	}
	else if (c == '*')
	{
		return CT_MUL;
	}
	else if (c == '/')
	{
		return CT_DIV;
	}
	else if (c == '^')
	{
		return CT_POW;
	}
	else if (c == ' ')
	{
		return CT_SPACE;
	}
	else
	{
		return NUM_CHAR_TYPES;
	}
}

}


struct Node
{
	Node* paths[NUM_CHAR_TYPES];
	TokenType tokenType;
};


Tokenizer::Tokenizer()
{
	m_root = newNode(TT_INVALID);
	
	Node* identifier = newNode(TT_IDENTIFIER);
	Node* num1 = newNode(TT_INTEGER);
	Node* num2 = newNode(TT_FLOAT);
	Node* inv2 = newNode(TT_INVALID);
	Node* comma = newNode(TT_COMMA);
	Node* op = newNode(TT_OPEN_PAREN);
	Node* cp = newNode(TT_CLOSE_PAREN);
	Node* eq = newNode(TT_EQUAL);
	Node* add = newNode(TT_ADD);
	Node* sub = newNode(TT_SUB);
	Node* mul = newNode(TT_MUL);
	Node* div = newNode(TT_DIV);
	Node* pow = newNode(TT_POW);
	
	connect(m_root, CT_SPACE, m_root);
	
	connect(m_root, CT_LETTER_OR_UNDERSCORE, identifier);
	connect(identifier, CT_LETTER_OR_UNDERSCORE, identifier);
	connect(identifier, CT_NUMBER, identifier);
	
	connect(m_root, CT_NUMBER, num1);
	connect(num1, CT_NUMBER, num1);
	connect(num1, CT_DOT, inv2);
	connect(inv2, CT_NUMBER, num2);
	connect(num2, CT_NUMBER, num2);
	
	connect(m_root, CT_COMMA, comma);
	connect(m_root, CT_OPEN_PAREN, op);
	connect(m_root, CT_CLOSE_PAREN, cp);
	connect(m_root, CT_EQUAL, eq);
	connect(m_root, CT_ADD, add);
	connect(m_root, CT_SUB, sub);
	connect(m_root, CT_MUL, mul);
	connect(m_root, CT_DIV, div);
	connect(m_root, CT_POW, pow);
}

Tokenizer::~Tokenizer()
{
	std::unordered_set<Node*> toDelete;
	deleteAll(m_root, toDelete);
	
	for (auto it = toDelete.begin(); it != toDelete.end(); it++)
	{
		delete *it;
	}
}

std::vector<Token> Tokenizer::tokenize(const std::string& line) const
{
	std::vector<Token> result;
	unsigned i = 0;
	
	while (i < line.size())
	{
		Token token;
		unsigned charsRead = readToken(line, i, token);
		
		if (token.type == TT_INVALID)
		{
			if (token.str.empty())
			{
				throw std::runtime_error(std::string("Invalid character: ") + line[i]);
			}
			else
			{
				throw std::runtime_error(std::string("Invalid token: ") + token.str);
			}
		}
		else
		{
			result.push_back(token);
			i += charsRead;
		}
	}
	
	return result;
}

Node* Tokenizer::newNode(TokenType tokenType)
{
	Node* n = new Node;
	n->tokenType = tokenType;
	for (unsigned i = 0; i < NUM_CHAR_TYPES; i++)
	{
		n->paths[i] = nullptr;
	}
	
	return n;
}

void Tokenizer::deleteAll(Node* n, std::unordered_set<Node*>& toDelete)
{
	if (n == nullptr or toDelete.count(n) > 0)
	{
		return;
	}
	else
	{
		toDelete.insert(n);
		for (unsigned i = 0; i < NUM_CHAR_TYPES; i++)
		{
			deleteAll(n->paths[i], toDelete);
		}
	}
}

void Tokenizer::connect(Node* from, CharType charType, Node* to)
{
	from->paths[charType] = to;
}

unsigned int Tokenizer::readToken(const std::string& line, unsigned pos, Token& result) const
{
	std::string tokenStr;
	const Node* actual = m_root;
	unsigned i;

	for (i = pos; i < line.size(); i++)
	{
		CharType type = getCharType(line[i]);
		
		if (type != NUM_CHAR_TYPES and actual->paths[type] != nullptr)
		{
			if (line[i] != ' ')
			{
				tokenStr.push_back(line[i]);
			}
			
			actual = actual->paths[type];
		}
		else
		{
			break;
		}
	}

	result = Token(actual->tokenType, tokenStr);
	return i - pos;
}
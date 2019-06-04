#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <vector>

enum TokenType
{
	TT_IDENTIFIER,
	TT_INTEGER,
	TT_FLOAT,
	TT_DOT,
	TT_OPEN_PAREN,
	TT_CLOSE_PAREN,
	TT_EQUAL,
	TT_ADD,
	TT_SUB,
	TT_MUL,
	TT_DIV,
	TT_INVALID,
	NUM_TOKEN_TYPES
};

struct Token
{
	Token() {}
	
	Token(TokenType type, const std::string& str) :
	type(type), str(str) {}
	
	TokenType type;
	std::string str;
};

typedef std::vector<Token> TokenList;
typedef std::vector<Token>::const_iterator TokenIterator;

#endif
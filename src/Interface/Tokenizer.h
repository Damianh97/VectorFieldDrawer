#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "Token.h"

#include <string>
#include <unordered_set>
#include <vector>


enum CharType
{
	CT_LETTER_OR_UNDERSCORE,
	CT_NUMBER,
	CT_DOT,
	CT_OPEN_PAREN,
	CT_CLOSE_PAREN,
	CT_EQUAL,
	CT_ADD,
	CT_SUB,
	CT_MUL,
	CT_DIV,
	CT_SPACE,
	NUM_CHAR_TYPES
};

struct Node;

class Tokenizer
{

public:

	Tokenizer();

	~Tokenizer();
	
	TokenList tokenize(const std::string& line) const;

private:

	Node* newNode(TokenType tokenType);
	
	void deleteAll(Node* n, std::unordered_set<Node*>& toDelete);

	void connect(Node* from, CharType charType, Node* to);

	unsigned int readToken(const std::string& line, unsigned pos, Token& result) const;
	
	Node* m_root;

};

#endif
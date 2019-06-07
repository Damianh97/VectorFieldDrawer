#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <memory>
#include <string>
#include <sstream>
#include <vector>

#include "Token.h"

class ExpressionVisitor;

//AST node representing abstract expression
class Expression
{
public:
	virtual void accept(ExpressionVisitor& v) = 0;
};

typedef std::unique_ptr<Expression> ExpressionPtr;

class BinaryExpression;
class ConstExpression;
class VarExpression;
class EvaluationExpression;
class UnaryMinusExpression;

class ExpressionVisitor
{
public:
	virtual void visit(BinaryExpression& e) = 0;
	virtual void visit(ConstExpression& e) = 0;
	virtual void visit(VarExpression& e) = 0;
	virtual void visit(EvaluationExpression& e) = 0;
	virtual void visit(UnaryMinusExpression& e) = 0;
};

/*----------Derived expression types----------*/

class BinaryExpression : public Expression
{
public:
	BinaryExpression(char op): op(op) {} //'left' and 'right' are left uninitialized
	virtual void accept(ExpressionVisitor& v)
	{
		v.visit(*this);
	}
public:
	ExpressionPtr left;
	ExpressionPtr right;
	char op;
};

class ConstExpression : public Expression
{
public:
	ConstExpression(float constant): constant(constant) {}
	virtual void accept(ExpressionVisitor& v)
	{
		v.visit(*this);
	}
public:
	float constant;
};

class VarExpression: public Expression
{
public:
	VarExpression(const std::string& name): varName(name) {}
	virtual void accept(ExpressionVisitor& v)
	{
		v.visit(*this);
	}
public:
	std::string varName;
};

class EvaluationExpression : public Expression
{
public:
	virtual void accept(ExpressionVisitor& v)
	{
		v.visit(*this);
	}
public:
	std::string funcName;
	std::vector<ExpressionPtr> args;
};

class UnaryMinusExpression : public Expression
{
public:
	UnaryMinusExpression(ExpressionPtr arg): arg(std::move(arg)) {}
	virtual void accept(ExpressionVisitor& v)
	{
		v.visit(*this);
	}
public:
	ExpressionPtr arg;
};

// An object used to parse a mathematical expressions
class ExpressionParser
{
public:
	// Builds a parser for the sequence of tokens [first, last)
	ExpressionParser(TokenIterator first, TokenIterator last);
	// Parses the expression and returns it as an AST.
	ExpressionPtr parse();
	// Returns the error message produced while parsing the expression
	std::string errorMsg();
	
private:
	// Parses from m_actualPos up to limit
	ExpressionPtr parse(TokenIterator limit);
	// Parses a non binary expression starting from m_actualPos,
	ExpressionPtr parseNonBinary(TokenIterator limit);
	// Parses from m_actualPos up to limit several comma-separated expressions
	void parseCommaSeparated(TokenIterator limit, std::vector<ExpressionPtr>& exprs);
	/* Receives an array of expressions where even positions contain non binary expressions
	 * and odd positions contain binary expressions with undefined operands. Converts the 
	 * range [start, end) of the array into a tree (i.e. an expression) respecting the 
	 * precedences of the binary operators.
	 *
	 * Example: nodes = ["x", "*", "y", "+", "y", "^", "2"]
	 * Output:
	 *         _+_
	 *        /   \
	 *       *     ^
	 *      / \   / \
	 *     x  y  y   2
	 *
	 * The array is modified because the pointers are moved. */
	ExpressionPtr makeTree(std::vector<ExpressionPtr>& nodes, std::size_t start, std::size_t end);
	
private:
	TokenIterator m_first; // Iterators received when constructed. Determine the range of tokens being parsed
	TokenIterator m_last;  //
	std::vector<TokenIterator> m_parens;
	TokenIterator m_actualPos; // Iterator to next token
	std::ostringstream m_errorStream; // Stream where error messages are written
};


#endif
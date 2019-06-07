#include "../src/Interface/Tokenizer.h"
#include "../src/Interface/Expression.h"
#include <iostream>

class ExpressionPrinter: public ExpressionVisitor
{
public:
	virtual void visit(BinaryExpression& e)
	{
		std::cout << e.op << " ";
		e.left->accept(*this);
		e.right->accept(*this);
	}
	virtual void visit(ConstExpression& e)
	{
		std::cout << e.constant << " ";
	}
	virtual void visit(VarExpression& e)
	{
		std::cout << e.varName << " ";
	}
	virtual void visit(EvaluationExpression& e)
	{
		std::cout << e.funcName << " ";
		for (auto& arg : e.args) arg->accept(*this);
	}
	virtual void visit(UnaryMinusExpression& e)
	{
		std::cout << "-" << " ";
		e.arg->accept(*this);
	}
private:
	
};

int main()
{
	Tokenizer tkzer;
	TokenList tokenized_example = tkzer.tokenize("2*x + 5*(y - H(u,v))");
	
	ExpressionParser parser(tokenized_example.begin(), tokenized_example.end());
	ExpressionPtr result = parser.parse();
	
	ExpressionPrinter printer;
	result->accept(printer);
}
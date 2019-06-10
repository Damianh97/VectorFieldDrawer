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
	std::string example = "2^x + 5.3*(y - H(t1,t2,t3)/2)";
	std::cout << "Expression: " << example << std::endl;
	
	Tokenizer tkzer;
	TokenList tokenized = tkzer.tokenize(example);
	ExpressionParser parser(tokenized.begin(), tokenized.end());
	ExpressionPtr result = parser.parse();
	
	std::cout << "Parsed expression in prefix notation: ";
	ExpressionPrinter printer;
	result->accept(printer);
	std::cout << std::endl;
}
#pragma once
#include "ExpressionManagerInterface.h"
#include <sstream>

class ExpressionManager :
	public ExpressionManagerInterface
{
public:
	ExpressionManager();
~ExpressionManager();

bool isBalanced(string);
string postfixToInfix(string);
string infixToPostfix(string);
string postfixEvaluate(string);


private:

	bool const isOpen(char);
	bool const isClose(char);
	bool  checkP(stack <char>&, char open);
	bool const isInt(string number);
	bool const isOperator(string symbol);
	int const getPrecedence(string symbol);
};


#include "token.h"

Token::Token(string newType, string newValue, int line)
{
	type = newType;
	tokenValue = newValue;
	lineNumber = line;
}

Token::~Token()
{
}

string Token::toString()
{
	string returnString = "(" + type + "," + "\"" + tokenValue + "\"" + "," + to_string(lineNumber) + ")";
	return returnString;
}

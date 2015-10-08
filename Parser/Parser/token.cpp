#include "token.h"

Token::Token(tokenType newType, string newValue, int line)
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
	string tokenTypeString;
	switch (type)
	{
	case (COMMA):
		tokenTypeString = "COMMA";
		break;
	case (PERIOD) :
		tokenTypeString = "PERIOD";
		break;
	case (Q_MARK) :
		tokenTypeString = "Q_MARK";
		break;
	case (LEFT_PAREN) :
		tokenTypeString = "LEFT_PAREN";
		break;
	case (RIGHT_PAREN) :
		tokenTypeString = "RIGHT_PAREN";
		break;
	case(COLON) :
		tokenTypeString = "COLON";
		break;
	case (COLON_DASH) :
		tokenTypeString = "COLON_DASH";
		break;
	case (MULTIPLY) :
		tokenTypeString = "MULTIPLY";
		break;
	case (ADD) :
		tokenTypeString = "ADD";
		break;
	case (SCHEMES) :
		tokenTypeString = "SCHEMES";
		break;
	case (FACTS) :
		tokenTypeString = "FACTS";
		break;
	case (RULES) :
		tokenTypeString = "RULES";
		break;
	case(QUERIES) :
		tokenTypeString = "QUERIES";
		break;
	case (myID) :
		tokenTypeString = "ID";
		break;
	case(mySTRING) :
		tokenTypeString = "STRING";
		break;
	case(COMMENT) :
		tokenTypeString = "COMMENT";
		break;
	case(UNDEFINED) :
		tokenTypeString = "UNDEFINED";
		break;
	case(myEOF) :
		tokenTypeString = "EOF";
		break;
	default:
		tokenTypeString = "NOPE";
		break;
	}
	string returnString = "(" + tokenTypeString + "," + "\"" + tokenValue + "\"" + "," + to_string(lineNumber) + ")";
	return returnString;
}

tokenType Token::getType()
{
	return type;
}

string Token::getValue()
{
	return tokenValue;
}

int Token::getLineNumber()
{
	return lineNumber;
}

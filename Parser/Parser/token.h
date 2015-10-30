#pragma once

#include <string>

using namespace std;

enum tokenType {
	COMMA,
	PERIOD,
	Q_MARK,
	LEFT_PAREN,
	RIGHT_PAREN,
	COLON,
	COLON_DASH,
	MULTIPLY,
	ADD,
	SCHEMES,
	FACTS,
	RULES,
	QUERIES,
	myID,
	mySTRING,
	COMMENT,
	UNDEFINED,
	myEOF
};

class Token
{
public:
	Token(tokenType newType, string newValue, int line);
	~Token();
	string toString();

	tokenType getType();
	string getValue();
	int getLineNumber();



//private:
	tokenType type;
	string tokenValue;
	int lineNumber;
};


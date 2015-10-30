

#pragma once
#include <iostream>
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
	SCHEMES,
	FACTS,
	RULES,
	QUERIES,
	ID,
	STRING,
	COMMENT,
	WHITESPACE
};

class Token {
public:

	string data;
	int lineNum;
	tokenType myType;

	Token(string, int, tokenType);
	Token();
	virtual ~Token();

	string getData();
	int getLine();
	tokenType getType();
	string getInfo();
};



#include "Token.h"
#include <iostream>
#include <string>

Token::Token(string d, int l, tokenType t) {
	data = d;
	lineNum = l;
	myType = t;
}

string Token::getData() {
	return data;
}

int Token::getLine() {
	return lineNum;
}

tokenType Token::getType() {
	return myType;
}

string Token::getInfo() {
	string s;
	//s = "(" + getType() +",\"" + getData() + "\"," + getLine() + ")";
	//s = "(" << getType() << ",\"" << getData() << "\"," << getLine();
	return s;
}

Token::Token(){}
Token::~Token() {}

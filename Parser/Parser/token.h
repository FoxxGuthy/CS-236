#pragma once

#include <string>

using namespace std;

class Token
{
public:
	Token(string newType, string newValue, int line);
	~Token();
	string toString();

private:
	string type;
	string tokenValue;
	int lineNumber;
};


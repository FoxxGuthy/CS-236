#pragma once
#include "token.h"
#include <sstream>
class Parameter
{
public:
	Parameter(string newData, tokenType T);
	virtual ~Parameter();

private:
	string info;
	tokenType type;
	string toString();

}; 
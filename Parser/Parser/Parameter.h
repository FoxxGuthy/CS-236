#pragma once
#include "token.h"
#include <vector>
#include <sstream>


class Parameter
{
public:
	Parameter(/*tokenType T*/);
	virtual ~Parameter();


	vector<string> data;
	tokenType type;
	string toString();

}; 

#pragma once
#include <iostream>
#include <sstream>
#include "Token.h"


using namespace std;

class Parameter {
public:

	Parameter(string, tokenType);
	virtual ~Parameter();

	string toString();//stringstream &);
//	tokenType deterType();

	string info;
	tokenType type; //id or string?

};


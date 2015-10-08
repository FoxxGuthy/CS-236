#include "Parameter.h"

//Will this handle expressions?
Parameter::Parameter(string newData, tokenType T)
{
	info = newData;
	type = T;
}

Parameter::~Parameter()
{
}

string Parameter::toString()
{

	return string();
}

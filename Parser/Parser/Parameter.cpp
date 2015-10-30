#include "Parameter.h"

//Will this handle expressions?
Parameter::Parameter( /*tokenType T*/)
{
	//type = T;
}

Parameter::~Parameter()
{
}

string Parameter::toString()
{
	stringstream ss;
	for (int i = 0; i < data.size(); i++)
	{
		ss << data[i];

		if (data[i] != data.back()) 
			ss << ",";
	}
	return ss.str();
}

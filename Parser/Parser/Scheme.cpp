#include "Scheme.h"



Scheme::Scheme()
{
}


Scheme::~Scheme()
{
}

string Scheme::toString()
{
	stringstream ss;
	ss << IDvec[0] << "(";
	for (int i = 1; i < IDvec.size(); i++) //start at second itme
	{
		ss << IDvec[i];
		if (IDvec[i] != IDvec.back()) //if its the last element, we don't need a comma
			ss << ",";
	}
	ss << ")";
	return ss.str();
}

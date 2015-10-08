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
	for (int i = 1; i < IDvec.size(); i++) 
	{
		ss << IDvec[i] << ",";
	}
	ss << ")";
	return ss.str();
}

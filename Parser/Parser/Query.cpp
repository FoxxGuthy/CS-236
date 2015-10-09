#include "Query.h"



Query::Query()
{
}


Query::~Query()
{
}

string Query::toString()
{
	stringstream ss;
	ss << queryPred[0].toString();
	for (int i = 1; i < queryPred.size(); i++)
	{
		ss << queryPred[i].toString();
	}
	ss << "?";
	return ss.str();
}

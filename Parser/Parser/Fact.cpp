#include "Fact.h"



Fact::Fact()
{
}


Fact::~Fact()
{
}

string Fact::toString()
{
	stringstream ss;
	ss << factStringVector[0] << "("; 
	for (int i = 1; i < factStringVector.size(); i++) //start at second itme
	{
		ss << factStringVector[i];
		if (factStringVector[i] != factStringVector.back())
			ss << ",";
	}
	ss << ")";
	return ss.str();
}

list<string> Fact::getList()
{
	//need to set the list equal to the values in the vector, minus index 0
	for (int j = 1; j < factStringVector.size(); j++)
	{
		factStringList.push_back(factStringVector[j]);
	}
	factStringList.sort();
	factStringList.unique();

	return factStringList;
}

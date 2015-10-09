#include "Rule.h"



Rule::Rule()
{
}


Rule::~Rule()
{
}

string Rule::toString()
{
	stringstream ss;
	ss << rulePredicateVec[0]->toString();
	ss << " :- ";
	for (int i = 1; i < rulePredicateVec.size(); i++)
	{
		ss << rulePredicateVec[i]->toString();
	}
	ss << "." << endl;
	return ss.str();
}

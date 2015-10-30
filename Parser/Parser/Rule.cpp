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
	ss << rulePredicateVec[0].toString() << ") :- ";
	for (int i = 1; i < rulePredicateVec.size(); i++)
	{
		ss << rulePredicateVec[i].toString();
		if (i < (rulePredicateVec.size() - 1))
		{
			ss << ","; //we have too many predicate breaks, so we get too many commas
		}
	}
	ss << ".";
	return ss.str();
}

#pragma once
#include "Predicate.h"

class Rule
{
public:
	Rule();
	~Rule();

	string toString();

	vector<Predicate> rulePredicateVec;
};


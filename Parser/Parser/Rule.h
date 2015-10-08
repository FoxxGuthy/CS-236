#pragma once
#include "Predicate.h"
class Rule
{
public:
	Rule();
	~Rule();

	string toString();

private:
	Predicate ruleHeadPred;
	Predicate rulePred;
};


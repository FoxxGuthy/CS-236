#pragma once
#include "Predicate.h"

class Query
{
public:
	Query();
	~Query();

	vector<Predicate> queryPred;

	string toString();
};


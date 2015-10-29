

#pragma once
#include "Predicate.h"

using namespace std;

class Query {
public:
	Query(Predicate);
	virtual ~Query();

	Predicate myPred;
};



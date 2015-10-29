
#pragma once
#include "Predicate.h"
#include "vector"

using namespace std;

class Rule {
public:
	Rule(Predicate);
	virtual ~Rule();

	void addPredicate(Predicate); // push parameter onto vector

	Predicate myPred;
	vector<Predicate> predList;
};


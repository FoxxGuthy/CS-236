

#pragma once
#include "Predicate.h"

using namespace std;

class Fact {
public:
	Fact(Predicate);
	virtual ~Fact();


	Predicate myPred;
};


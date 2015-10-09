#pragma once
#include "Parameter.h"


class Predicate
{
public:
	Predicate();
	~Predicate();

//private:

	//headPredicate	->	ID LEFT_PAREN ID idList RIGHT_PAREN
	// HP ->			ID ( ID idList)

	//predicate	->	ID LEFT_PAREN parameter parameterList RIGHT_PAREN

	//possibly add type to make decisions on whether its a head or regular

	vector<string> PredIDsVec;
	vector<Parameter*> PredParmVec;

	string toString();
};


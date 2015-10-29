

#pragma once
#include "Parameter.h"
#include "vector"
#include <sstream>


using namespace std;

class Predicate {
public:
	Predicate();
	Predicate(string);
	virtual ~Predicate();

	void addParameter(Parameter); // push parameter onto vector
	void setInfo(string);

	string toString();//stringstream &);


	string info; //id
	vector<Parameter> paramList;


};


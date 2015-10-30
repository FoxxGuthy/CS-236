

#pragma once
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <set>
#include "Scheme.h"
#include "Tuple.h"
#include "Predicate.h"

using namespace std;

class Relation {
public:
	Relation();
	virtual ~Relation();

	string name;
	Scheme scheme;
	set<Tuple> myTuples;
	vector<Predicate> mySchemes;
	vector<Predicate> myFacts;
	vector<Predicate> myQueries;

	Relation select(size_t, string);

	Relation select(size_t, size_t);

	Relation project(vector<int>&);

	void rename(string, size_t);

	void setNameSchema(vector<Predicate>&, size_t);

	void setTuples(vector<Predicate>&, size_t);

	void setName(vector<Predicate>&, size_t);

	Relation evalParams(vector<Predicate>&, size_t, Relation);
	void evalParamID(Relation&, Relation&, size_t&, size_t&, size_t&,
			vector<string>&, vector<int>&, bool);
	void thereIsADuplicate(Relation&, Relation&, size_t&, size_t&, size_t&,
			vector<string>&, vector<int>&, bool);

	size_t isThereAnother(string, vector<Parameter>, size_t);

	void printRelation(size_t, stringstream&, vector<size_t>&);

};


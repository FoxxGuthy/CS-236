

#pragma once
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "Relation.h"
#include "Predicate.h"

using namespace std;

class Database {
public:
	Database();
	virtual ~Database();

	vector<Relation> myRelations, toPrint;

	vector<Predicate> mySchemes;
	vector<Predicate> myFacts;
	vector<Predicate> myQueries;


	void addRelation(Relation);

	void fill(vector<Predicate>&,vector<Predicate>&,vector<Predicate>&);

	void evalQueries(vector<Predicate>&);

	void printResults(stringstream&);//, vector<Relation>&, vector<Predicate>&);
	void printParams(vector<size_t>&, vector<string>&, bool, size_t, stringstream&);

};


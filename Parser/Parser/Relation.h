#pragma once
#include "Tuple.h"
#include "Parameter.h"
#include "Scheme.h"
#include "Fact.h"
#include "Query.h"
#include <set>
#include <sstream>

using namespace std;

class Relation
{
public:
	Relation();
	~Relation();

	string name;
	Scheme rScheme;
	vector<Parameter> Schema;
	set<Tuple> Tuples;

	void setSchema(vector<Scheme>& s, size_t index);
	void setTuples(vector<Fact>&f, size_t index);

	Relation evalParams(vector<Query>&q, size_t i, Relation source);
	
	Relation select(size_t index, string value);
	Relation select(size_t index1, size_t index2);
	Relation project(vector<int>& myIndexVector);

	

	// void rename(string s, size_t j);
	// void setName(vector<Query>& q, size_t i);
	// void evalParamID(Relation& source, Relation& two);
	// void thereIsADuplicate
	// isThereAnother


	void PrintRelation(size_t i, stringstream& out, vector<size_t>& varIndex);

};


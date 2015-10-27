#pragma once
#include "Tuple.h"
#include "Parameter.h"
#include "Scheme.h"
#include "Fact.h"
#include "Query.h"
#include <set>

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
};


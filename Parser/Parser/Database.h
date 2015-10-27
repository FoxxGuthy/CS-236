#pragma once
#include "DatalogProgram.h"
#include "Relation.h"

using namespace std;

class Database
{
public:
	Database();
	~Database();

	DatalogProgram DP;
	vector<Relation> myRelations;

	void addRelation(Relation r);
	void fillRelations();

	void evalQueries();

	string printQueryResults();
};


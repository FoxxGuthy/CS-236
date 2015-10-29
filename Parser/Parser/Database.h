#pragma once
#include "DatalogProgram.h"
#include "Relation.h"
#include <sstream>

using namespace std;

class Database
{
public:
	Database();
	~Database();

	DatalogProgram DP;
	vector<Relation> myRelations;
	vector<Relation> toPrint;

	void addRelation(Relation r);
	void fillRelations();

	void evalQueries();

	void printQueryResults(stringstream& out);
	void printParams(vector<size_t>& varIndex, vector<string>& variablestoBePrinted, bool toAdd, size_t i, stringstream& out);
};


#pragma once
#include <vector>
#include <list>
#include <sstream>
#include "Scheme.h"
//#include "Fact.h"
//#include "Rule.h"
//#include "Query.h"

using namespace std;

class DatalogProgram
{
public:
	DatalogProgram();
	~DatalogProgram();

	vector<Scheme*> schemeVector;
	//vector<Fact> factVector;
	//vector<Rule> ruleVector;
	//vector<Query> queryVector;

	list<string> domainList; //domain are strings from FACTS. has to be sorted and no duplicates. Luckily "lists has this built i

	string schemeToString();
	//string factToString();
	//string ruleToString();
	//string queryToString();
	//string domainToString();

	string ToString();

};


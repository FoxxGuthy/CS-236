

#pragma once
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include "Relation.h"
#include "Predicate.h"
#include "Rule.h"
#include "Node.h"

using namespace std;

class Database {
public:
	Database();
	virtual ~Database();

	vector<Relation> myRelations, toPrint, newFacts;

	vector<Predicate> mySchemes;
	vector<Predicate> myFacts;
	vector<Predicate> myQueries;
	vector<Rule> myRules;
	int numPasses;


	void addRelation(Relation);

	void fill(vector<Predicate>&,vector<Predicate>&,vector<Predicate>&, vector<Rule>&);

	void evalQueries(vector<Predicate>&, bool addNewFacts);

	void printResults(stringstream&);//, vector<Relation>&, vector<Predicate>&);
	void printParams(vector<size_t>&, vector<string>&, bool, size_t numOfStringsDetected, size_t, stringstream&);

	//LAB 4
	Relation find(string& n);
	int getTupleCount(); //used to find size of the schemes, if they've stopped growing, we can stop iterating
	void useRules(vector<Rule>& rules); //begins process to use rules
	void evalRules(vector<Rule> &rules); //loops thru rules, checking for tuples to add
	void evalRulesMultiFacts(Relation& one, Relation& two, Relation& three, //more than 1 new fact added. helper to evalRules
		Relation& projected, Relation& joined, Relation& toJoin, size_t i);
	void checkDuplicateSchemes(vector<Relation> & r);
	bool contains(string& curr, vector<string>& currAttributes);
	void removeDupSchemes(Relation& r, vector<size_t>& removeIndex);

	//lab 5

	map<string, Node> myGraph;
	int postCntr;

	void createGraph(vector<Predicate>&, vector<Rule>&);
	void printGraph(stringstream&);
	//void printQuery(stringstream&, vector<Predicate>&);
	void depthFirst(string&);
	//void printPostNums(stringstream&);
	//void printRuleOrder(stringstream&, string);
	//void printBackwardEdges(stringstream&);
	//void continuePrintingBackwardEdges(string&, Node&, int&, stringstream&);
	void reset();
	string convertInt(int);

};


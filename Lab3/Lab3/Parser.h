/*
 * Parser.h
 *
 *  Created on: Sep 20, 2012
 *      Author: adamjc
 */

#pragma once
#include "Token.h"
#include "vector"
#include "Predicate.h"
#include "Rule.h"


using namespace std;

class Parser {
public:
	Parser();
	virtual ~Parser();

	void parse(vector<Token>&);
	void match(tokenType);

	void parseFactList(vector<Token>&);
	void parseRuleList(vector<Token>&);

	void parseQueryList(vector<Token>&);
	void parsePredicateList(vector<Token>&, Rule&);

	void parseParamList(vector<Token>&, Predicate&);
	void parseSchemeList(vector<Token>&);

	void advanceIndex();
	void fillmyToken(vector<Token>&);

	void print(stringstream &);
	void printSchemes(stringstream&);

	void printFacts(stringstream&);
	void printRules(stringstream&);

	void printQueries(stringstream&);
	void printDomain(stringstream&);

	int myIndex;


	//for lab4
	vector<Predicate>& getSchemes();
	vector<Predicate>& getFacts();
	vector<Predicate>& getQueries();
	vector<Rule>& getRules();

};


#pragma once
#include <vector>
#include "token.h"
#include "DatalogProgram.h"



using namespace std;

//I want to get out of that massive main from Lab 1, so we're gonna try to build as much as possible here
class Parser
{
public:
	Parser();
	~Parser();

	void match(tokenType T, vector<string>& YourMom);
	void match(tokenType T); //overload that sucker
	void setCurrentToken(vector<Token> toParse);
	int currentIndex;

	DatalogProgram AllthatData;

	string toString();


	//parse datalog
	void parseDataLog(vector<Token> toParse);


	//parse scheme
	void parseScheme(vector<Token> toParse);
	//schemelist
	void parseSchemeList(vector<Token> toParse);

	
	//ID
	void parseIDList(vector<Token> toParse, vector<string> &scheme_OR_pred);
	
	//parse fact
	void parseFact(vector<Token> toParse);
	//parse list
	void parseFactList(vector<Token> toParse);

	//parse rule 
	void parseRule(vector<Token> toParse);
	//parse rule list
	void parseRuleList(vector<Token> toParse);

	//prase head pred
	void parseHeadPred(vector<Token> toParse, vector<string>& PredIDs);
	//parse pred
	void parsePred(vector<Token> toParse, vector<Predicate>& ruleORqueryPRED);
	//parse list
	void parsePredList(vector<Token> toParse, vector<Predicate >& listOfPredicates, Rule tempRule);

	//parse param
	void parseParameter(vector<Token> toParse, vector<Parameter > & ruleORqueryParam);
	//parse param list
	void parseParameterList(vector<Token> toParse, vector<Parameter > & ruleORqueryParam);

	//parse expression
	void parseExpression(vector<Token> toParse, vector<Parameter > & ruleORqueryParam);

	//parse operator
	void parseOperator(vector<Token> toParse, vector<Parameter > moreStuff);

	//parse query 
	void parseQuery(vector<Token> toParse);
	//querylist
	void parseQueryList(vector<Token> toParse);
	
	//parse string list
	void parseStringList(vector<Token> toParse, vector<string> &factStrings);
};


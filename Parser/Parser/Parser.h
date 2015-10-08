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

	//Token currentToken;

	//void parse(vector<Token> &toParse);
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
	void parseIDList(vector<Token> toParse, vector<string> scheme_OR_pred);
	//parse fact 
	//parse list

	//parse rule 
	//parse rule list

	//prase head pred
	void parseHeadPred(vector<Token> toParse);
	//parse pred
	//parse list

	//parse param
	//parse param list

	//parse expression	

	//parse operator
	void parseOperator(vector<Token> toParse);

	//parse query 
	//querylist
	
	//parse string list
};


#include "Parser.h"

Token currentToken(COMMA, ",", 0);
static int schemeVecIndex = 0;

Parser::Parser()
{
	currentIndex = 0;
}


Parser::~Parser()
{
}

void Parser::match(tokenType T, vector<string> &storage ) //for storing IDs, specifically in schemes
{
	if (T == currentToken.getType())
	{
		if (T == myID) //IDs are really important to things, other chars can be reput in toString()
		{
			storage.push_back(currentToken.getValue());
		}
		currentIndex++;
	}
	else
	{
		throw(currentToken);
	}
}

void Parser::match(tokenType T) //some times we just need to check if its the same, for example COLON, SCHEMES, etch
{
	if (T == currentToken.getType())
	{
		currentIndex++;
	}
	else
	{
		throw(currentToken);
	}
}

void Parser::setCurrentToken(vector<Token> toParse)
{
	currentToken.type = toParse.at(currentIndex).getType();
	currentToken.tokenValue= toParse.at(currentIndex).getValue();
	currentToken.lineNumber = toParse.at(currentIndex).getLineNumber();
}

string Parser::toString()
{
	return AllthatData.ToString();
}

void Parser::parseDataLog(vector<Token> toParse)
{
	//-----------------------------------------------------------
	//RULE 1 SCHEMES COLON scheme schemeList
	setCurrentToken(toParse);
	match(SCHEMES);

	setCurrentToken(toParse);
	match(COLON);

	parseScheme(toParse);
	parseSchemeList(toParse);

	//-----------------------------------------------------------
	//RULE 2 FACTS COLON factList

	//-----------------------------------------------------------
	//RULE 3 RULES COLON ruleList

	//-----------------------------------------------------------
	//RULE 4  QUERIES COLON query queryList

	//-----------------------------------------------------------
	
	setCurrentToken(toParse);
	match(myEOF); //if we don't look for EOF we'll throw at the very end
}

void Parser::parseScheme(vector<Token> toParse)
{
	Scheme* temp = new Scheme(); //make a new Scheme class
	//ID LEFT_PAREN ID idList RIGHT_PAREN
	setCurrentToken(toParse);
	match(myID, temp->IDvec); //pass the scheme's ID vec in
	
	setCurrentToken(toParse);
	match(LEFT_PAREN);

	setCurrentToken(toParse);
	match(myID, temp->IDvec);

	parseIDList(toParse, temp->IDvec);

	setCurrentToken(toParse);
	match(RIGHT_PAREN);

	AllthatData.schemeVector.push_back(temp);
	//delete temp?
}

void Parser::parseSchemeList(vector<Token> toParse)
{
	//scheme schemeList | lambda
	//scheme list will always be followed by FACTS so we'll break there
	while (toParse[currentIndex].getType() != FACTS  && toParse[currentIndex].getType() != myEOF) //since we don't have facts.. check for EOF? //loop executing an extra time
	{
		parseScheme(toParse);
		//AllthatData.schemeVector.push_back()
		parseSchemeList(toParse);
	}
}

void Parser::parseIDList(vector<Token> toParse, vector<string> scheme_OR_pred)
{
	//COMMA ID idList | lambda 
	//idList is always followed by a ) so we'll break there
	while (toParse[currentIndex].getType() != RIGHT_PAREN)
	{
		//how to store the ID? //store to that scheme class we made earlier
		setCurrentToken(toParse);
		match(COMMA);

		setCurrentToken(toParse);
		//needs to know where to pass to
		match(myID, scheme_OR_pred);

		parseIDList(toParse, scheme_OR_pred); //the recursion
	}
}

void Parser::parseHeadPred(vector<Token> toParse)
{
}

void Parser::parseOperator(vector<Token> toParse)
{
}

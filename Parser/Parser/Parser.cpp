#include "Parser.h"

Token currentToken(COMMA, ",", 0);
static int schemeVecIndex = 0;

static int RuleIndex = 0; //how many rules

static int PredVecIndex = 0; //how many predicates in a rule

static int PredPramIndex = 0;
static int ParamDataIndex = 0;

static int queryIndex = 0;

Parser::Parser()
{
	currentIndex = 0;
}

Parser::~Parser()
{
}

//used for storing scheme IDs in vectors
//used for storing fact strings (the domain) in vectors
void Parser::match(tokenType T, vector<string> &storage )
{
	if (T == currentToken.getType())
	{
		storage.push_back(currentToken.getValue());
		currentIndex++;
	}
	else
	{
		throw(currentToken);
	}
}

//some times we just need to check if its the same, for example COLON, SCHEMES, etch
//matches terminal token types
void Parser::match(tokenType T) 
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

	setCurrentToken(toParse);
	match(FACTS);

	setCurrentToken(toParse);
	match(COLON);

	parseFactList(toParse);

	//-----------------------------------------------------------
	//RULE 3 RULES COLON ruleList

	setCurrentToken(toParse);
	match(RULES);

	setCurrentToken(toParse);
	match(COLON);

	parseRuleList(toParse);

	//-----------------------------------------------------------
	//RULE 4  QUERIES COLON query queryList

	setCurrentToken(toParse);
	match(QUERIES);

	setCurrentToken(toParse);
	match(COLON);

	parseQuery(toParse);

	parseQueryList(toParse);

	//-----------------------------------------------------------
	
	setCurrentToken(toParse);
	match(myEOF); //if we don't look for EOF we'll throw at the very end
}

void Parser::parseScheme(vector<Token> toParse)
{
	Scheme  temp;//make a new Scheme class
	//ID LEFT_PAREN ID idList RIGHT_PAREN
	setCurrentToken(toParse);
	match(myID, temp.IDvec); //pass the scheme's ID vec in
	
	setCurrentToken(toParse);
	match(LEFT_PAREN);

	setCurrentToken(toParse);
	match(myID, temp.IDvec);

	parseIDList(toParse, temp.IDvec);

	setCurrentToken(toParse);
	match(RIGHT_PAREN);

	AllthatData.schemeVector.push_back(temp);
}

void Parser::parseSchemeList(vector<Token> toParse)
{
	//scheme schemeList | lambda
	//scheme list will always be followed by FACTS so we'll break there
	while (toParse[currentIndex].getType() != FACTS) 
	{
		parseScheme(toParse);
		parseSchemeList(toParse);
	}
}

void Parser::parseIDList(vector<Token> toParse, vector<string> &scheme_OR_pred)
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

void Parser::parseFact(vector<Token> toParse)
{
	// ID LEFT_PAREN STRING stringList RIGHT_PAREN PERIOD

	Fact  tempFact;

	setCurrentToken(toParse);
	match(myID, tempFact.factStringVector); //the fact ID

	setCurrentToken(toParse);
	match(LEFT_PAREN);

	setCurrentToken(toParse);
	match(mySTRING, tempFact.factStringVector); //its vector of strings. This is the domain. 

	parseStringList(toParse, tempFact.factStringVector);

	setCurrentToken(toParse);
	match(RIGHT_PAREN);
	
	setCurrentToken(toParse);
	match(PERIOD);

	AllthatData.factVector.push_back(tempFact);
}

void Parser::parseFactList(vector<Token> toParse)
{
	//fact factlist | lambda
	while (toParse[currentIndex].getType() != RULES)
	{
		parseFact(toParse);
		parseFactList(toParse);
	}
}

void Parser::parseRule(vector<Token> toParse)
{
	//rule    	->	headPredicate COLON_DASH predicate predicateList PERIOD

	Rule tempRule;

	PredVecIndex = 0;
	PredPramIndex = 0;
	ParamDataIndex = 0;

	tempRule.rulePredicateVec.push_back(Predicate());
	int index = tempRule.rulePredicateVec.size() - 1;

	parseHeadPred(toParse, tempRule.rulePredicateVec[index].PredIDsVec);

	//tempRule.rulePredicateVec.push_back(Predicate());

	PredVecIndex++;

	setCurrentToken(toParse);
	match(COLON_DASH);

	parsePred(toParse, tempRule.rulePredicateVec);

	parsePredList(toParse, tempRule.rulePredicateVec, tempRule);

	setCurrentToken(toParse);
	match(PERIOD);

	AllthatData.ruleVector.push_back(tempRule);
	RuleIndex++;
}

void Parser::parseRuleList(vector<Token> toParse)
{
	//ruleList	->	rule ruleList | lambda
	while (toParse[currentIndex].getType() != QUERIES)
	{
		parseRule(toParse);

		parseRuleList(toParse);
	}
}

void Parser::parseHeadPred(vector<Token> toParse, vector<string>& PredIDs)
{
	//headPredicate->ID LEFT_PAREN ID idList RIGHT_PAREN
	while (toParse[currentIndex].getType() != COLON_DASH) //head Pred always followed by colonDash
	{
		setCurrentToken(toParse);
		match(myID, PredIDs);

		setCurrentToken(toParse);
		match(LEFT_PAREN);

		setCurrentToken(toParse);
		match(myID, PredIDs);

		parseIDList(toParse, PredIDs);

		setCurrentToken(toParse);
		match(RIGHT_PAREN);
	}
}

void Parser::parsePred(vector<Token> toParse, vector<Predicate >& ruleORqueryPRED)
{
	ParamDataIndex = 0;
	ruleORqueryPRED.push_back( Predicate());
	//predicate	->	ID LEFT_PAREN parameter parameterList RIGHT_PAREN
	setCurrentToken(toParse);
	match(myID, ruleORqueryPRED[PredVecIndex].PredIDsVec);

	setCurrentToken(toParse);
	match(LEFT_PAREN);

	parseParameter(toParse, ruleORqueryPRED[PredVecIndex].PredParmVec);

	parseParameterList(toParse, ruleORqueryPRED[PredVecIndex].PredParmVec);

	setCurrentToken(toParse);
	match(RIGHT_PAREN);

	PredVecIndex++;
}

void Parser::parsePredList(vector<Token> toParse, vector<Predicate >& listOfPredicates, Rule tempRule)
{
	//predicateList	->	COMMA predicate predicateList | lambda
	while (toParse[currentIndex].getType() != PERIOD)
	{
		setCurrentToken(toParse);
		match(COMMA);

		parsePred(toParse, listOfPredicates);

		parsePredList(toParse, listOfPredicates, tempRule);
	}
}

void Parser::parseParameter(vector<Token> toParse, vector<Parameter > & ruleORqueryParam)
{
	int index = ParamDataIndex;
	ruleORqueryParam.push_back(Parameter());
	index = ruleORqueryParam.size() - 1;
	setCurrentToken(toParse);

	switch (currentToken.getType())
	{
	case mySTRING:
		match(mySTRING, ruleORqueryParam[index].data);
		break;
	case myID:
		match(myID, ruleORqueryParam[index].data);
		break;
	default:
		parseExpression(toParse, ruleORqueryParam);
		break;
	}
	ParamDataIndex++;
}

void Parser::parseParameterList(vector<Token> toParse, vector<Parameter > & ruleORqueryParam)
{
	//parameterList	-> 	COMMA parameter parameterList | lambda
	while (toParse[currentIndex].getType() != RIGHT_PAREN)
	{
		setCurrentToken(toParse);
		match(COMMA);

		parseParameter(toParse, ruleORqueryParam);

		parseParameterList(toParse, ruleORqueryParam);
	}
}

void Parser::parseExpression(vector<Token> toParse, vector<Parameter > & ruleORqueryParam)
{
	//expression	-> 	LEFT_PAREN parameter operator parameter RIGHT_PAREN
	setCurrentToken(toParse);
	match(LEFT_PAREN);

	parseParameter(toParse, ruleORqueryParam);

	parseOperator(toParse, ruleORqueryParam);

	parseParameter(toParse, ruleORqueryParam);

	setCurrentToken(toParse);
	match(RIGHT_PAREN);
}

void Parser::parseOperator(vector<Token> toParse, vector<Parameter > moreStuff)
{
	//operator	->	ADD | MULTIPLY
	
	setCurrentToken(toParse);

	int index = 0;
	moreStuff.push_back(Parameter());
	index = moreStuff.size() - 1;

	switch (currentToken.getType())
	{
	case ADD:
		match(ADD, moreStuff[index].data);
		break;
	default:
		match(MULTIPLY, moreStuff[index].data); //this will test for multiply or just throw if its broken
		break;
	}
	//PredPramIndex++;
}

void Parser::parseQuery(vector<Token> toParse)
{
	//query->predicate Q_MARK

	Query tempQuery;
	PredVecIndex = 0;
	PredPramIndex = 0;
	ParamDataIndex = 0;

	parsePred(toParse, tempQuery.queryPred);

	setCurrentToken(toParse);
	match(Q_MARK);

	AllthatData.queryVector.push_back(tempQuery);
	queryIndex++;
}

void Parser::parseQueryList(vector<Token> toParse)
{
	//queryList	->	query queryList | lambda

	//nothing follows Queries
	while (toParse[currentIndex].getType() != myEOF)
	{
		parseQuery(toParse);
		parseQueryList(toParse);
	}
}

void Parser::parseStringList(vector<Token> toParse, vector<string> &factStrings)
{
	//stringList	->	COMMA STRING stringList | lambda 
	//string list is always followed by a right paren so we'll check for that to end recursion
	while (toParse[currentIndex].getType() != RIGHT_PAREN)
	{
		setCurrentToken(toParse);
		match(COMMA);

		setCurrentToken(toParse);
		match(mySTRING, factStrings);

		parseStringList(toParse, factStrings);
	}
}


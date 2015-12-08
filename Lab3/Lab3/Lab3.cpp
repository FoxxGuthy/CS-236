
//#pragma once
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "Token.h"
#include "Scanner.h"
#include "Parser.h"
#include "Predicate.h"
#include "Database.h"

using namespace std;

int main(int argc, char* argv[])
{
	ifstream in;
	stringstream out;
	in.open(argv[1]);


	Scanner scanner;
	scanner.scan(in);

	//for lab 2

	try
	{
		vector<Token>& myTokens = scanner.getTokens();
		Parser parser;

		parser.parse(myTokens);

		//for lab3

		vector<Predicate>& mySchemes = parser.getSchemes();
		vector<Predicate>& myFacts = parser.getFacts();
		vector<Predicate>& myQueries = parser.getQueries();
		vector<Rule>& myRules = parser.getRules();

		Database dbase;

		//lab 5

		dbase.createGraph(myQueries, myRules);
		dbase.printGraph(out);
		dbase.printQuery(out, myQueries);

		//end of lab 5 additions
		dbase.fill(mySchemes, myFacts, myQueries, myRules);
		dbase.printResults(out);
		cout << out.str();

	}

	catch (Token e)
	{
		out << "Failure!" << endl;
		string myType;

		switch (e.getType())
		{
		case(0) : myType = "COMMA";
			break;
		case(1) : myType = "PERIOD";
			break;
		case(2) : myType = "Q_MARK";
			break;
		case(3) : myType = "LEFT_PAREN";
			break;
		case(4) : myType = "RIGHT_PAREN";
			break;
		case(5) : myType = "COLON";
			break;
		case(6) : myType = "COLON_DASH";
			break;
		case(7) : myType = "SCHEMES";
			break;
		case(8) : myType = "FACTS";
			break;
		case(9) : myType = "RULES";
			break;
		case(10) : myType = "QUERIES";
			break;
		case(11) : myType = "ID";
			break;
		case(12) : myType = "STRING";
			break;
		}

		out << "  (" << myType << ",\"" << e.getData() << "\"," << e.getLine() << ")" << endl;
	}

	return 0;
}



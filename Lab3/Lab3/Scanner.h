

#pragma once
#include "Token.h"
#include <vector>

using namespace std;

class Scanner {
public:
	Scanner();
	virtual ~Scanner();

	string data;
	string type;
	int line = 1;
	int eLineNum = 0;
	vector<Token> myTokens;
	bool eOccurred = false;

	void scan(ifstream &);
	void errorOccurred();
	bool isAPunctuation(string, ifstream&);
	bool isACommaPeriodQuestionParen(string);
	bool isSpaceCharacter(string);
	void punctuation(string);
	void print(stringstream&);
	void isString(string, ifstream&);
	void isColon(string, ifstream&);
	void error(int, stringstream&);
	void isID(char, ifstream&);
	bool checkForPunctuationInisID(char);
	bool isQuoteColon(char, string, string, ifstream&);
	void comment(ifstream&);
	string intToString(int);
	int stringToInt(string);
	string typeToString(int);
	void isSpecial(string);

	//for lab 2
	vector<Token>& getTokens();

};


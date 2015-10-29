

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "Scanner.h"
#include "Token.h"

using namespace std;



Scanner::Scanner() {
}

void Scanner::scan(ifstream& file_in) {
//	if (file_in.is_open()) {

	while (!file_in.eof()) {

		char e = file_in.get();
		data += e;

		if (data == "\n") {
			line++;
			data.clear();
		} else if (isAPunctuation(data, file_in)) {
			data.clear();
		} else if (isalpha(e)) {
			isID(e, file_in);
			data.clear();
		} else if (isspace(e)) {
			//cout << "Found whitespace" << endl;
			data.clear();
		} else if (e == -1) {
			//cout << "found end of file" << endl;
			data.clear();
			break;
		} else {
			//cout << "found something else wtf" << endl;
			//error(line);
			errorOccurred();
			data.clear();
		}
	}
}

void Scanner::errorOccurred() {
	eOccurred = true;
	if (eLineNum == 0)
		eLineNum = line;
}

bool Scanner::isAPunctuation(string data, ifstream& file_in) {

	if (isACommaPeriodQuestionParen(data)) {
		punctuation(data);
		data.clear();
		return true;
	}
	if (data == ":") {
		isColon(data, file_in);
		data.clear();
		return true;
	}
	if (data == "'") {
		isString(data, file_in);
		data.clear();
		return true;
	}
	if (data == "#") {
		comment(file_in);
		data.clear();
		return true;
	}
	return false;
}
bool Scanner::isACommaPeriodQuestionParen(string data) {
	if (data == "," || data == "." || data == "?" || data == "(" || data == ")")
		return true;
	else
		return false;
}
void Scanner::punctuation(string info) {
	string data = info;
	Token newToken;

	if (data == ",") {
		newToken = Token(data, line, COMMA);
	} else if (data == ".") {
		newToken = Token(data, line, PERIOD);
	} else if (data == "?") {
		newToken = Token(data, line, Q_MARK);
	} else if (data == "(") {
		newToken = Token(data, line, LEFT_PAREN);
	} else if (data == ")") {
		newToken = Token(data, line, RIGHT_PAREN);
	}

	myTokens.push_back(newToken);
}
void Scanner::print(stringstream &file_out) {
	string t;
	for (size_t i = 0; i < myTokens.size(); i++) {
		t = typeToString(myTokens.at(i).getType());
		file_out << "(" << t << +",\"" << myTokens.at(i).getData() << +"\","
				<< myTokens.at(i).getLine() << +")" << endl;
	}
	if (eOccurred == false) {
		file_out << "Total Tokens = " << myTokens.size() << endl;
	} else {
		error(eLineNum, file_out);
		myTokens.clear();
	}
}
void Scanner::isString(string s, ifstream&in) {
	string data;
	char c;
	bool t = true;

	while (t) {
		c = in.get();
		if (c == '\'') {
			Token newToken(data, line, STRING);
			myTokens.push_back(newToken);
			t = false;
		} else if (c == '\n' || c == -1) {
			//error(line);
			errorOccurred();
			t = false;
		} else {
			data += c;
		}
	}
}
void Scanner::isColon(string s, ifstream &in) {
	string data = s;
	char c = in.peek();
	Token newToken;

	if (c == '-') {
		c = in.get();
		data += c;
		newToken = Token(data, line, COLON_DASH);
	} else
		newToken = Token(data, line, COLON);

	myTokens.push_back(newToken);

}
void Scanner::error(int l, stringstream &file_out) {
	if (eOccurred == true) {
		file_out << "Input Error on line " << l << endl;
		myTokens.clear();
	}
}

void Scanner::isID(char c, ifstream& in) {
	char value = c;
	string data, temp;
	bool t = true;

	data += value;

	while (t) {
		value = in.get();
		if (isalnum(value)) {
			data += value;
		} else if (isspace(value)) {
			isSpecial(data);
			data.clear();

			if (value == '\n') {
				line++;
			}
			t = false;
		} else if (checkForPunctuationInisID(value)) {
			isSpecial(data);
			data.clear();
			temp += value;
			punctuation(temp);
			temp.clear();
		} else if (isQuoteColon(value, data, temp, in)) {
			data.clear();
			temp.clear();
		} else if (value == -1) {
			isSpecial(data);
			data.clear();
			t = false;
		} else {
			//error(line);
			isSpecial(data);
			errorOccurred();
			t = false;
		}
	}
}

bool Scanner::checkForPunctuationInisID(char data) {
	if (data == ',' || data == '.' || data == '?' || data == '(' || data == ')')
		return true;
	else
		return false;
}

bool Scanner::isQuoteColon(char value, string data, string temp, ifstream& in) {
	if (value == '\'') {
		isSpecial(data);
		data.clear();
		temp += value;
		isString(temp, in);
		temp.clear();
		return true;
	} else if (value == ':') {
		isSpecial(data);
		data.clear();
		temp += value;
		isColon(temp, in);
		temp.clear();
		return true;
	}
	return false;
}

void Scanner::isSpecial(string s) {
	if (eOccurred == false) {
		string data = s;

		if (data == "Schemes") {
			Token newToken(data, line, SCHEMES);
			myTokens.push_back(newToken);
			data.clear();
		} else if (data == "Facts") {
			Token newToken(data, line, FACTS);
			myTokens.push_back(newToken);
			data.clear();
		} else if (data == "Rules") {
			Token newToken(data, line, RULES);
			myTokens.push_back(newToken);
			data.clear();
		} else if (data == "Queries") {
			Token newToken(data, line, QUERIES);
			myTokens.push_back(newToken);
			data.clear();
		} else if (isSpaceCharacter(data)) {

		} else {
			Token newToken(data, line, ID);
			myTokens.push_back(newToken);
			data.clear();
		}
	}
}

bool Scanner::isSpaceCharacter(string data) {
	if (data == "\n" || data == "\t" || data == "" || data == " ")
		return true;
	else
		return false;
}

void Scanner::comment(ifstream &in) {
	char c = in.get();
	bool t = true;

	while (t) {
		if (c == '\n' || c == -1) {
			if (c == '\n') {
				line++;
			}
			t = false;
		} else {
			c = in.get();
		}
	}
}
string Scanner::intToString(int t) {
	ostringstream strm;
	strm << t;
	return strm.str();
}
int Scanner::stringToInt(string s) {
	int i = atoi(s.c_str());
	return i;
}
string Scanner::typeToString(int t) {
	string type;

	switch (t) {
	case 0:
		type = "COMMA";
		return type;
	case 1:
		type = "PERIOD";
		return type;
	case 2:
		type = "Q_MARK";
		return type;
	case 3:
		type = "LEFT_PAREN";
		return type;
	case 4:
		type = "RIGHT_PAREN";
		return type;
	case 5:
		type = "COLON";
		return type;
	case 6:
		type = "COLON_DASH";
		return type;
	case 7:
		type = "SCHEMES";
		return type;
	case 8:
		type = "FACTS";
		return type;
	case 9:
		type = "RULES";
		return type;
	case 10:
		type = "QUERIES";
		return type;
	case 11:
		type = "ID";
		return type;
	case 12:
		type = "STRING";
		return type;
	default:
		type = "WTF";
		return type;
	}
}

Scanner::~Scanner() {
}

vector<Token>& Scanner::getTokens() {
	return myTokens;
}


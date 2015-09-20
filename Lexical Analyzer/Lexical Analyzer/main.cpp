#include <iostream> // cout. communicatio with the humans
#include <vector> // used to store each token and its info
#include <fstream> // used for writing and reading files
#include <string>

using namespace std;

int const FOUND = 1;
int const NOT_FOUND = 0;
string const BLANK = "";

//----------------------------------------------------------------------------------------------------------------------
//                      FSM FUNCTIONS

//COMMA CAPTURE
string Comma(char* position)
{
	if (*position == ',')
	{
		string s(1, *position); //cast the char as a string, so we can call string functions and keep this consistent
		return s; //for more complicated FSM ths will be the whole string, then we'll check the length. 
	}
	else
		return BLANK;
}

string Period(char* position)
{
	if (*position == '.')
	{
		string s(1, *position); //cast the char as a string, so we can call string functions and keep this consistent
		return s; //for more complicated FSM ths will be the whole string, then we'll check the length. 
	}
	else
		return BLANK;
}

string Q_mark(char* position)
{
	if (*position == '?')
	{
		string s(1, *position); //cast the char as a string, so we can call string functions and keep this consistent
		return s; //for more complicated FSM ths will be the whole string, then we'll check the length. 
	}
	else
		return BLANK;
}

string Left_paren(char* position)
{
	if (*position == '(')
	{
		string s(1, *position); //cast the char as a string, so we can call string functions and keep this consistent
		return s; //for more complicated FSM ths will be the whole string, then we'll check the length. 
	}
	else
		return BLANK;
}

string right_paren(char* position)
{
	if (*position == ')')
	{
		string s(1, *position); //cast the char as a string, so we can call string functions and keep this consistent
		return s; //for more complicated FSM ths will be the whole string, then we'll check the length. 
	}
	else
		return BLANK;
}

//colon

//colon-dash

string Multiply(char* position)
{
	if (*position == '*')
	{
		string s(1, *position); //cast the char as a string, so we can call string functions and keep this consistent
		return s; //for more complicated FSM ths will be the whole string, then we'll check the length. 
	}
	else
		return BLANK;
}

string Add(char* position)
{
	if (*position == '+')
	{
		string s(1, *position); //cast the char as a string, so we can call string functions and keep this consistent
		return s; //for more complicated FSM ths will be the whole string, then we'll check the length. 
	}
	else
		return BLANK;
}

//schemes

//facts

//rules

//queries

//ID

//strings
//@param: char position
//will look for a single quote, then go to the next single quote not followed by a quote
//ignore white space
//returning blank if it doesn't find the end single quote
string StringFSM(char* position)
{
	if (*position == '\'')
	{
		char* finder = position;
		finder++; //for our analysis we don't want finder pointing at the quote, we need it at the first whatever
		string tokenString = "'";
		while (*finder != '\'' && (finder != NULL)) //need to prevent ptr from going out of bounds
		{
			tokenString += *finder; //append next value
			//if the next two chars are a single quote, move forward 2 and don't end the string
			char* next0 = finder;
			next0++; //now points to char after finder

			if (next0 == NULL) //EOF before end of string
				return BLANK;

			char* next1 = next0;
			next1++; //now points to char after next0

			if (next1 == NULL) //EOF before end of string
				return BLANK;

			if ((*next0 == '\'') && (*next1 == '\''))
			{
				tokenString += "''";
				finder++;
				finder++;
			}
			else
				finder++;
		}
		if (*finder == '\'')
		{
			tokenString += "'"; //this should be the closing quote to the string
			return tokenString;
		}
		else if (finder == NULL) //reached end of file before we found another single quote
			return BLANK;
	}
	else
		return BLANK;
}

//COMMENT DETECTOR (SINGLE OR MULTILINE)
string Comment(char* position)
{
	string commentString = "#";
	char* finder = position;
	finder++;
	if (*position == '#') //its a comment, enter here
	{
		if (*finder == '|') // multiline comments
		{
			commentString += "|";
			char* next0 = finder;
			next0++; //now points to char after finder

			if (next0 == NULL) //EOF before end of string
				return BLANK;

			char* next1 = next0;
			next1++; //now points to char after next0

			if (next1 == NULL) //EOF before end of string
				return BLANK;


			while (*finder != '|' && *next0 != '#')
			{
				commentString += *finder;
				finder++;
				next0++;

				if (next0 == NULL) //EOF before end of string
					return BLANK;

				next1++;

				if (next1 == NULL) //EOF before end of string
					return BLANK;
			}
			commentString += "|#";
			return commentString;
		}
		else //single line comment
		{
			while (*finder != '\n')
			{
				commentString += *finder;
				finder++;
			}
			return commentString;
		}
	}
	else
		return BLANK;
}

//EOF



//NEWLINE CAPTURE
//@param: position in string, and the currentLine count, passed in by reference
// if char is new line: position++, newLineCount++
void NewLineDetect(char*& position, int& newLineCount)
{
	if (*position == '\n')
	{
		position++;
		newLineCount++;
	}
}

/*
CAPTURE UNDEFINED-----------------BROKE AF
since this will be at the end, it really only needs to check what is as a precaution
checks if its an alphanumber just in case
if it is not, moves position along
casts the char back to an string and returns it
else returns empty string
*/
string Undefined(char* position) //MAY NOT WORK WITH BROKEN STRINGS/COMMENTS EXAMPLE: 'A STRING
{
	//check is it #| with no end?
	//is it 'string with end?
	if (!isalnum(*position)) //its not a number or alpha, so just move along
	{
		string s(1, *position); //make string first, or we lose that period when we move the position
		return s;
	}
	else
		return BLANK;
}

void FindLongestLength(string accepted, int& longestLength)
{
	if (accepted.length() > longestLength)
		longestLength = accepted.length();

}

//----------------------------------------------------------------------------------------
//THE MAIN

int main(int argc, char* argv[])
{
	//----------------------------------------------------------------------------
	//OPEN THE FILE

	ifstream inputFile;
	inputFile.open(argv[1]);

	char* position; //this variable will point to where we are in the input string

	if (!inputFile.is_open()) //check that the file actually opened
		cout << "not open" << endl;

	string inputString;
	string line;
	while (getline(inputFile, line)) //while we're not at the end of the file, take in input
	{
		inputString += line + "\n"; //add the file line by line into the inputString. Getline removes the \n, so we add it back in!
	}
	inputFile.close(); //close the File. we don't need it anymore

	position = &inputString[0]; // pointer to first part of string

	int lineNumber = 1;

	//------------------------------------------------------------------------------------------------------------------------
	//THE SCANNER (THE WHILE LOOP)

	//this will the strings of the tokens list: example: (QUERIES,"Queries",1)
	vector<string> tokenList;

	//iterate thru the inputString, however big it is
	for (unsigned i = 0; i < inputString.size(); i++)
	{
		int longestLength = 0; //will store the longest

		//--------------------------------------------------------------------------------------------------------------------
		//FINDING LONGEST ACCEPTER

		string CommaFind = Comma(position); //so we don't have to keep calling the comma function, save time
		FindLongestLength(CommaFind, longestLength);

		string PeriodFind = Period(position);
		FindLongestLength(PeriodFind, longestLength);

		string Q_markFind = Q_mark(position);
		FindLongestLength(Q_markFind, longestLength);

		string Left_parenFind = Left_paren(position);
		FindLongestLength(Left_parenFind, longestLength);

		string Right_parenFind = right_paren(position);
		FindLongestLength(Right_parenFind, longestLength);

		//colon

		//colon dash

		string MultiFind = Multiply(position);
		FindLongestLength(CommaFind, longestLength);

		string AddFind = Add(position);
		FindLongestLength(AddFind, longestLength);

		//schemes

		//facts

		//rules

		//queries

		//ID

		string StringFSMFind = StringFSM(position);
		FindLongestLength(StringFSMFind, longestLength);

		string CommentFind = Comment(position);
		FindLongestLength(CommentFind, longestLength);

		//FindLongestLength(Undefined(position), longestLength);

		//--------------------------------------------
		//TOKENIZING LOGIC & VECTOR STORAGE

		//put keyword first, so that they get priority

		string tokenString = "";

		if (longestLength == CommaFind.length())  //when not all machines are active and we get a zero, it'll just cycle thru here...0 == 0; TRUE!
		{
			tokenString = "(COMMA, \",\", " + to_string(lineNumber) + ")";
			tokenList.push_back(tokenString);
			position++;
		}

		else if (longestLength == PeriodFind.length())
		{
			tokenString = "(PERIOD, \".\", " + to_string(lineNumber) + ")";
			tokenList.push_back(tokenString);
			position++;
		}

		else if (longestLength == Q_markFind.length())
		{
			tokenString = "(Q_MARK, \"?\", " + to_string(lineNumber) + ")";
			tokenList.push_back(tokenString);
			position++;
		}

		else if (longestLength == Left_parenFind.length())
		{
			tokenString = "(LEFT_PAREN, \"(\", " + to_string(lineNumber) + ")";
			tokenList.push_back(tokenString);
			position++;
		}

		else if (longestLength == Right_parenFind.length())
		{
			tokenString = "(RIGHT_PAREN, \")\", " + to_string(lineNumber) + ")";
			tokenList.push_back(tokenString);
			position++;
		}

		else if (longestLength == MultiFind.length())
		{
			tokenString = "(MULTIPLY, \"*\", " + to_string(lineNumber) + ")";
			tokenList.push_back(tokenString);
			position++;
		}

		else if (longestLength == AddFind.length())
		{
			tokenString = "(ADD, \"+\", " + to_string(lineNumber) + ")";
			tokenList.push_back(tokenString);
			position++;
		}

		else if (longestLength == StringFSMFind.length())
		{
			tokenString = "(STRING, \"" + StringFSMFind + "\", " + to_string(lineNumber) + ")";
			tokenList.push_back(tokenString);
			for (int i = 0; i < longestLength; i++)
			{
				if (*position != '\n') //we want the newline to survive so we can up the number
					position++; //move position forward as many are in the string
			}
		}

		else if (longestLength == CommentFind.length())
		{
			tokenString = "(COMMENT, \"" + CommentFind + "\", " + to_string(lineNumber) + ")";
			tokenList.push_back(tokenString);
			for (int i = 0; i < longestLength; i++)
			{
				if (*position != '\n') //we want the newline to survive so we can up the number
					position++; //move position forward as many are in the string
			}
		}

		//else if (longestLength == Undefined(position).length())
		//{
		//	string tokenString = "(UNDEFINED, \"";
		//	
		//	//The problem is that *position isn't a string, its a char, therefore we can't use string operations on it
		//	//we need to make a substring from where position is pointing for the length of the stuff
		//	for (unsigned j = 0; j < Undefined(position).length(); j++) // BROKE AF
		//	{
		//		tokenString += *position;
		//		position++;
		//	}
		//	tokenString += "\", " + to_string(lineNumber) + ")";
		//	tokenList.push_back(tokenString);
		//}

		NewLineDetect(position, lineNumber); //getline isn't grabbing the newline characters...eff. what if we add them back in?


		//some algorithm to see who returned the greatest length, and tokenize that value, and move position along that far

		//how to remember what the longest accepted was is easy, how to rember what accept it might be tricker
		//I guess you only have to compare certain things?
		// the KWs VS ID mostly...
		//would it be too much to automatically have the simples ones push back their token? YES. If those symbols are in a comment or string we're boned
	}

	for (auto S : tokenList)
	{
		cout << S << endl; //eventually write to a string
	}
	/*
	SCANNER ALGORITHM

	while input is not empty do
		si := the longest prefix that some Di accepts;
		k := |si|;
		if k > 0 then
			j := min  {i:|si|=k and Di reads si};
			remove sj from input;
			perform the jth action
		else
			move one character from input to output (bad)
		end
	end
	*/
	return 0;
}
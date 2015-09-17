#include <iostream> // cout. communicatio with the humans
#include <vector> // used to store each token and its info
#include <fstream> // used for writing and reading files
#include <string>


//#include "FSM.h"

using namespace std;

/* FSM Functions for each token in the language
Each will return an int saying how long the token was. 
If it was not accepted, will return 0

Also need to track line numbers from input
	EOF will the last line

input and output will be to a text file

Priority for FSM output of equal length: KW, ID, Strings

usefull functions: 

isspace() whitespace checker
isalpha()
isalnum alphanumeric checker
isdigit ()


*/
const int FOUND = 1;
const int NOT_FOUND = 0;

int comma(char alphaNum) //assumes that a single character is passed in, if say, ",Schemes" is passed in, this won't work
{
	if (alphaNum == ',')
		return FOUND;
	else
		return NOT_FOUND;
}

int period(string alphaNum)
{
	if (alphaNum == ".")
		return FOUND;
	else
		return NOT_FOUND;
}

int q_mark( string alphaNum)
{
	if (alphaNum == "?")
		return FOUND;
	else
		return NOT_FOUND;
}

int left_paren( string alphaNum)
{
	if (alphaNum == "(")
		return FOUND;
	else
		return NOT_FOUND;
}

int right_paren( string alphaNum)
{
	if (alphaNum == ")")
		return FOUND;
	else
		return NOT_FOUND;
}

int colon( string alphaNum)
{
	if (alphaNum == ":")
		return FOUND;
	else
		return NOT_FOUND;
}

int colon_dash( string alphaNum)
{
	return NULL;
}

int multiply( string alphaNum)
{
	if (alphaNum == "*")
		return FOUND;
	else
		return NOT_FOUND;
}

int add( string alphaNum)
{
	if (alphaNum == "+")
		return FOUND;
	else
		return NOT_FOUND;
}

//Keyword
int schemes( string alphaNum) //must be case sensitive. Would we call it recursively to look at the next letter?
{
	return NULL;
}

//Keyword
int rules( string alphaNum)
{
	return NULL;
}

//Keyword
int queries( string alphaNum)
{
	return NULL;
}

//ID
int id( string alphaNum)
{
	return NULL;
}

int stringFSM( string alphaNum)
{
	return NULL;
}

int comment( string alphaNum)
{
	return NULL;
}

const bool TESTING = true;

int main(int argc, char* argv[])
{
	ifstream inputFile;
	inputFile.open(argv[1]);

	//next we begin to parse in input. 
	//use get line to look at whole line at a time, then we'll look at each character, passing it to the FSM Functions
	//after each getline, iterate or line counter, which will be used in each stored token

	 string line;
	int lineNumber = 0;

	while (getline(inputFile, line))
	{
		lineNumber++;

		// if there's a blank line, what will get line do?

		//now line holds the entire string, now we pass it to each of the FSM, one character at a time. 
		//we iterate line
		//once we go thru all the functions we look at who "captured the most"?

		for (unsigned i = 0; i < line.size(); i++)
		{
			comma(line[i]);
			//all other FSMs

			//peek function

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


	}

		

return 0;
}
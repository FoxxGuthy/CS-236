#include <iostream> // cout. communicatio with the humans
#include <vector> // used to store each token and its info
#include <fstream> // used for writing and reading files
#include <string>

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
		position++; //move position to the next char
		string s(1, *position); //cast the char as a string, so we can call string functions and keep this consistent
		return s; //for more complicated FSM ths will be the whole string, then we'll check the length. 
	}
	else
		return BLANK;
}

//NEWLINE CAPTURE
//@param: position in string, and the currentLine count, passed in by reference
// if char is new line: position++, newLineCount++
void NewLineDectect(char* position, int& newLineCount) 
{
	if (*position == '\n')
	{
		position++;
		newLineCount++;
	}
}

/*
CAPTURE UNDEFINED 
since this will be at the end, it really only needs to check what is as a precaution
checks if its an alphanumber just in case
if it is not, moves position along
casts the char back to an string and returns it
else returns empty string
*/
string Undefined(char* position)
{
	if (!isalnum(*position)) //its not a number or alpha, so just move along
	{
		position++;
		string s(1, *position);
		return s;
	}
	else
		return BLANK;
}

int main(int argc, char* argv[])
{
	ifstream inputFile;
	inputFile.open(argv[1]);

	char* position; //this variable will point to where we are in the input string

	if (!inputFile.is_open()) //check that the file actually opened
		cout << "not open" << endl;

	string inputString;
	string line;
	while (getline (inputFile, line)) //while we're not at the end of the file, take in input
	{
		inputString += line; //add the file line by line into the inputString
	}
	inputFile.close(); //close the File. we don't need it anymore

	position = &inputString[0]; // pointer to first part of string

	int lineNumber = 0;

	

		// if there's a blank line, what will get line do?

		//now line holds the entire string, now we pass it to each of the FSM, one character at a time. 
		//we iterate line
		//once we go thru all the functions we look at who "captured the most"?

		

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
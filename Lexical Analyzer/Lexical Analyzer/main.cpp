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
















//***************************************
//IMPORTANT NOTE: CAN'T AUTOMATICALLY MOVE THE POSITION POINTER FOR THE MORE COMPLEX FSM
//IT'LL HAVE TO PEEK OR SOMETHING THEN MOVE IT AT THE END OF THE FOR LOOP















//NEWLINE CAPTURE
//@param: position in string, and the currentLine count, passed in by reference
// if char is new line: position++, newLineCount++
void NewLineDetect(char* position, int& newLineCount)
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
		inputString += line /*+ "\n"*/; //add the file line by line into the inputString. Getline removes the \n, so we add it back in!
	}
	inputFile.close(); //close the File. we don't need it anymore

	position = &inputString[0]; // pointer to first part of string

	int lineNumber = 1;

	cout << inputString;
	//----------------------------------------------------------------------------
	//THE SCANNER (THE WHILE LOOP)

	//this will the strings of the tokens list: example: (QUERIES,"Queries",1)
	vector<string> tokenList;

	//iterate thru the inputString, however big it is
	for (unsigned i = 0; i < inputString.size(); i++)
	{
		int longestLength = 0; //will store the longest

		//------------------------------------------------
		//FINDING LONGEST ACCEPTER

		//FindLongestLength(Comma(position), longestLength);

		FindLongestLength(Undefined(position), longestLength);

		//--------------------------------------------
		//TOKENIZING LOGIC & VECTOR STORAGE

		//if (longestLength == Comma(position).length()) //this doesn't work.. there are a lot of single length things. and if it hasn't found anything yet, both are zero... crap
		//{
		//	string tokenString = "(COMMA, \",\", " + to_string(lineNumber) + ")";
		//	tokenList.push_back(tokenString);
		//	position++;
		//}

		if (longestLength == Undefined(position).length())
		{
			

			//I'm proud of this next line: so we declare the type, then we take a substring of inputString from position, that is the length of the string
			string tokenString = "(UNDEFINED, ";
			
			//The problem is that *position isn't a string, its a char, therefore we can't use string operations on it
			//we need to make a substring from where position is pointing for the length of the stuff
			tokenString += *position[0, Undefined(position).length()]; // BROKE AF
			tokenString +=  to_string(lineNumber) + ")";
			tokenList.push_back(tokenString);

			position += Undefined(position).length();// move position forward how ever many are in the 
		}

		NewLineDetect(position, lineNumber); //getline isn't grabbing the newline characters...eff. what if we add them back in?
		

		//some algorithm to see who returned the greatest length, and tokenize that value, and move position along that far

		//how to remember what the longest accepted was is easy, how to rember what accept it might be tricker
		//I guess you only have to compare certain things?
		// the KWs VS ID mostly...
		//would it be too much to automatically have the simples ones push back their token? YES. If those symbols are in a comment or string we're boned
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
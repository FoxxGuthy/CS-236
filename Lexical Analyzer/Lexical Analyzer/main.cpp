


#include <string>
#include <iostream> // cout. communicatio with the humans
#include <vector> // used to store each token and its info
#include <fstream> // used for writing and reading files

#include "FSM.h"

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

const bool TESTING = true;

int main()
{
	std::string fileLocation;

	//Gets File Location from user
	cout << "Enter file (include directory tree):" << endl;
	getline(cin, fileLocation);

	//creats ifstream (used to open files)
	ifstream inputFile;
	//opens the File
	inputFile.open(fileLocation.c_str());
	//checks to see if file is open, if it is not, reasks for file location
	while (!inputFile.is_open())
	{
		cout << "File did not open. Enter File Location: ";
		getline(cin, fileLocation);
		inputFile.open(fileLocation.c_str());
	}

	

	//next we begin to parse in input. 
	//use get line to look at whole line at a time, then we'll look at each character, passing it to the FSM Functions
	//after each getline, iterate or line counter, which will be used in each stored token

	std::string line;
	int lineNumber = 0;

	while (getline(inputFile, line))
	{
		lineNumber++;
		//now line holds the entire string, now we pass it to each of the FSM, one character at a time. 
		//we iterate line

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
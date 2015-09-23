#include <iostream> // cout. communicatio with the humans
#include <vector> // used to store each token and its info
#include <fstream> // used for writing and reading files
#include <string>

using namespace std;

int const FOUND = 1;
int const NOT_FOUND = 0;
string const BLANK = "";

//-----------------------------------------------
//FILE OPEN FUNCTION
//don't really need this, but it will lower the complexity of main


//can also make print function for for loop at the end. Not necesaarry, but lowers complexity in main
void Output(vector <string> tokens)
{
    for (auto S : tokens)
    {

        cout << S << endl;
    }

    cout << "Total Tokens = " << tokens.size() << endl;
}

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

string Colon(char* position)
{
    if (*position == ':')
    {
        string s(1, *position); //cast the char as a string, so we can call string functions and keep this consistent
        return s; //for more complicated FSM ths will be the whole string, then we'll check the length.
    }
    else
        return BLANK;
}

string ColonDash(char* position)
{
    char* finder = position;
    finder++;
    if (*position == ':' && *finder == '-')
    {
        return ":-"; //for more complicated FSM ths will be the whole string, then we'll check the length.
    }
    else
        return BLANK;
}

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
string Schemes(char* position)
{
    //since position isn't passed in by reference, we can play with all we want
    char* ptr1 = position; //S
    char* ptr2 = ++position; //c
    char* ptr3 = ++position; //h
    char* ptr4 = ++position; //e
    char* ptr5 = ++position; //m
    char* ptr6 = ++position; //e
    char* ptr7 = ++position; //s
    if (*ptr1 == 'S' && *ptr2 == 'c' && *ptr3 == 'h' && *ptr4 == 'e' && *ptr5 == 'm' && *ptr6 == 'e' && *ptr7 == 's')
        return "Schemes";
    else
        return BLANK;
}

//facts
string Facts(char* position)
{
    //since position isn't passed in by reference, we can play with all we want
    char* ptr1 = position; //F
    char* ptr2 = ++position; //a
    char* ptr3 = ++position; //c
    char* ptr4 = ++position; //t
    char* ptr5 = ++position; //s

    if (*ptr1 == 'F' && *ptr2 == 'a' && *ptr3 == 'c' && *ptr4 == 't' && *ptr5 == 's')
        return "Facts";
    else
        return BLANK;
}

//rules
string Rules(char* position)
{
    //since position isn't passed in by reference, we can play with all we want
    char* ptr1 = position; //R
    char* ptr2 = ++position; //u
    char* ptr3 = ++position; //l
    char* ptr4 = ++position; //e
    char* ptr5 = ++position; //s

    if (*ptr1 == 'R' && *ptr2 == 'u' && *ptr3 == 'l' && *ptr4 == 'e' && *ptr5 == 's')
        return "Rules";
    else
        return BLANK;
}

//queries
string Queries(char* position)
{
    //since position isn't passed in by reference, we can play with all we want
    char* ptr1 = position; //Q
    char* ptr2 = ++position; //u
    char* ptr3 = ++position; //e
    char* ptr4 = ++position; //r
    char* ptr5 = ++position; //i
    char* ptr6 = ++position; //e
    char* ptr7 = ++position; //s
    if (*ptr1 == 'Q' && *ptr2 == 'u' && *ptr3 == 'e' && *ptr4 == 'r' && *ptr5 == 'i' && *ptr6 == 'e' && *ptr7 == 's')
        return "Queries";
    else
        return BLANK;
}

//ID
string ID(char* position)
{
    string IDstring;
    if (isalpha(*position))
    {
        while (isalnum(*position))
        {
            IDstring += *position;
            position++;
        }
        return IDstring;
    }
    else
        return BLANK;
}

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
        char* next0 = finder;
        next0++; //now points to char after finder
        char* next1 = next0;
        next1++; //now points to char after next0
        while (*finder != '\'' && (*finder != '\0') && *next1 != '\0' ) //need to prevent ptr from going out of bounds... may probalby still go out of bounds...
        {
            tokenString += *finder; //append next value
            //if the next two chars are a single quote, move forward 2 and don't end the string



            if ((*next0 == '\'') && (*next1 == '\''))
            {
                tokenString += "''";
                finder++; //move to the first single quote
                finder++; // mov to second single quote
                finder++; //now pointing at the letter after the "apostrophe"
                next0++;
                next0++;
                next0++;
                next1++;
                next1++;
                next1++;
            }
            else
            {
                finder++;
                next0++; //now points to char after finder
                next1++; //now points to char after next0
            }


        }
        if (*finder == '\'')
        {
            tokenString += "'"; //this should be the closing quote to the string
            return tokenString;
        }
        else
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
            //commentString += "|";
            char* next0 = finder;
            next0++; //now points to char after finder

            char* next1 = next0;
            next1++; //now points to char after next0

            if (*next1 == '\0') //EOF before end of string
                return BLANK;


            while (*finder != '|' || *next0 != '#')
            {
                commentString += *finder;
                finder++;
                next0++;

                next1++;

                if (*next1 == '\0') //EOF before end of string
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

void whiteSpace(char* &position)
{
    while (*position == ' ')
    {
        position++;
    }
    //could also use isspace
}


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
CAPTURE UNDEFINED
*/
string Undefined(char* position) //MAY NOT WORK WITH BROKEN STRINGS/COMMENTS EXAMPLE: 'A STRING
{
    //check is it #| with no end?
    //is it 'string with end?
    if (StringFSM(position) == "" && *position == '\'') //its starts with a ' but its not a string, so its an undefined string!
    {
        string s;
        while (*position != '\0')
        {
            s += *position;
            position++;
        }
        return s;
    }

    else if (Comment(position) == "" && *position == '#') //a multiline comment that hits EOF
    {
        string C;
        while (*position != '\0')
        {
            C += *position;
            position++;
        }
        return C;
    }

    else if (!isalnum(*position)) //its not a number or alpha, so just move along
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

string SimpleFSM(int& longestLength, string &tokenString, vector <string> &tokenList, int &lineNumber, char* &position) //to cut down complexity, this will call the simple FSMs
{
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


    string MultiFind = Multiply(position);
    FindLongestLength(MultiFind, longestLength);

    string AddFind = Add(position);
    FindLongestLength(AddFind, longestLength);


    //COMMA
    if (longestLength == CommaFind.length())  //when not all machines are active and we get a zero, it'll just cycle thru here...0 == 0; TRUE!
    {
        tokenString = "(COMMA,\",\"," + to_string(lineNumber) + ")";
        tokenList.push_back(tokenString);
        position++;
    }

    //PERIOD
    else if (longestLength == PeriodFind.length())
    {
        tokenString = "(PERIOD,\".\"," + to_string(lineNumber) + ")";
        tokenList.push_back(tokenString);
        position++;
    }

    //Q_MARK
    else if (longestLength == Q_markFind.length())
    {
        tokenString = "(Q_MARK,\"?\"," + to_string(lineNumber) + ")";
        tokenList.push_back(tokenString);
        position++;
    }

    //LEFT_PARAN
    else if (longestLength == Left_parenFind.length())
    {
        tokenString = "(LEFT_PAREN,\"(\"," + to_string(lineNumber) + ")";
        tokenList.push_back(tokenString);
        position++;
    }

    //RIGHT_PARAN
    else if (longestLength == Right_parenFind.length())
    {
        tokenString = "(RIGHT_PAREN,\")\"," + to_string(lineNumber) + ")";
        tokenList.push_back(tokenString);
        position++;
    }

    //MULTIPLY
    else if (longestLength == MultiFind.length())
    {
        tokenString = "(MULTIPLY,\"*\"," + to_string(lineNumber) + ")";
        tokenList.push_back(tokenString);
        position++;
    }

    //ADD
    else if (longestLength == AddFind.length())
    {
        tokenString = "(ADD,\"+\"," + to_string(lineNumber) + ")";
        tokenList.push_back(tokenString);
        position++;
    }

    return tokenString;
}

string MiddleFSM(int& longestLength, string &tokenString, vector <string> &tokenList, int &lineNumber, char* &position)
{
    //neeed to take just 1 out of simple, then test taking a few from complex

    //COLON String
    string  ColonFind = Colon(position);
    FindLongestLength(ColonFind, longestLength);

    //COLON_DASH_STRING
    string ColonDashFind = ColonDash(position);
    FindLongestLength(ColonDashFind, longestLength);

    //stringstring
    string StringFSMFind = StringFSM(position);
    FindLongestLength(StringFSMFind, longestLength);

    //COLON
    if (longestLength == ColonFind.length())
    {
        tokenString = "(COLON,\":\"," + to_string(lineNumber) + ")";
        tokenList.push_back(tokenString);
        position++;
    }

    //COLON_DASH
    else if (longestLength == ColonDashFind.length())
    {
        tokenString = "(COLON_DASH,\":-\"," + to_string(lineNumber) + ")";
        tokenList.push_back(tokenString);
        position++;
        position++;
    }

    //STRING
    else if (longestLength == StringFSMFind.length())
    {
        tokenString = "(STRING,\"" + StringFSMFind + "\"," + to_string(lineNumber) + ")";
        tokenList.push_back(tokenString);
        for (int i = 0; i < longestLength; i++)
        {
            if (*position == '\n')
                lineNumber++;

            //if (*position != '\n') //we want the newline to survive so we can up the number
            position++; //move position forward as many are in the string
        }
    }


    return tokenString;
}

string ComplexFSM(int& longestLength, string &tokenString, vector <string> &tokenList, int &lineNumber, char* &position)
{
    string ColonDashFind = ColonDash(position);
    FindLongestLength(ColonDashFind, longestLength);

    //schemes
    string SchemesFind = Schemes(position);
    FindLongestLength(SchemesFind, longestLength);

    //facts
    string FactsFind = Facts(position);
    FindLongestLength(FactsFind, longestLength);

    //rules
    string RulesFind = Rules(position);
    FindLongestLength(RulesFind, longestLength);

    //queries
    string QueriesFind = Queries(position);
    FindLongestLength(QueriesFind, longestLength);

    //ID
    string IDFind = ID(position);
    FindLongestLength(IDFind, longestLength);


    string CommentFind = Comment(position);
    FindLongestLength(CommentFind, longestLength);

    string UndefinedFind = Undefined(position);
    FindLongestLength(UndefinedFind, longestLength);

    //SCHEMES
    if (longestLength == SchemesFind.length())
    {
        tokenString = "(SCHEMES,\"" + SchemesFind + "\", " + to_string(lineNumber) + ")";
        tokenList.push_back(tokenString);
        for (int i = 0; i < longestLength; i++)
        {
            if (*position != '\n') //we want the newline to survive so we can up the number
                position++; //move position forward as many are in the string
        }
    }

    //FACTS
    else if (longestLength == FactsFind.length())
    {
        tokenString = "(FACTS,\"" + FactsFind + "\", " + to_string(lineNumber) + ")";
        tokenList.push_back(tokenString);
        for (int i = 0; i < longestLength; i++)
        {
            if (*position != '\n') //we want the newline to survive so we can up the number
                position++; //move position forward as many are in the string
        }
    }

    //RULES
    else if (longestLength == RulesFind.length())
    {
        tokenString = "(RULES,\"" + RulesFind + "\", " + to_string(lineNumber) + ")";
        tokenList.push_back(tokenString);
        for (int i = 0; i < longestLength; i++)
        {
            if (*position != '\n') //we want the newline to survive so we can up the number
                position++; //move position forward as many are in the string
        }
    }

    //QUERIES
    else if (longestLength == QueriesFind.length())
    {
        tokenString = "(QUERIES,\"" + QueriesFind + "\"," + to_string(lineNumber) + ")";
        tokenList.push_back(tokenString);
        for (int i = 0; i < longestLength; i++)
        {
            if (*position != '\n') //we want the newline to survive so we can up the number
                position++; //move position forward as many are in the string
        }
    }



    //COMMENT
    else if (longestLength == CommentFind.length())
    {
        tokenString = "(COMMENT,\"" + CommentFind + "\"," + to_string(lineNumber) + ")";
        tokenList.push_back(tokenString);
        for (int i = 0; i < longestLength; i++)
        {
            if (*position == '\n')
                lineNumber++;

            position++; //move position forward as many are in the string
        }
    }

    //ID
    else if (longestLength == IDFind.length())
    {
        tokenString = "(ID,\"" + IDFind + "\"," + to_string(lineNumber) + ")";
        tokenList.push_back(tokenString);
        for (int i = 0; i < longestLength; i++)
        {
            if (*position != '\n') //we want the newline to survive so we can up the number
                position++; //move position forward as many are in the string
        }
    }

    //UNDEFINED
    else if (longestLength == UndefinedFind.length())
    {
        string linecount = to_string(lineNumber);
        string tokenString = "(UNDEFINED,\"";
        for (unsigned j = 0; j < UndefinedFind.length(); j++) // BROKE AF
        {
            if (*position == '\n')
                lineNumber++;

            tokenString += *position;
            position++;
        }
        tokenString += "\"," + linecount + ")"; //that way it counts the first line number
        tokenList.push_back(tokenString);
    }

    return tokenString;
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

    //    if (!inputFile.is_open()) //check that the file actually opened
    //        cout << "not open" << endl;

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

    //iterate thru the inputString, however big it is
    for (unsigned i = 0; i < (inputString.size() - 1); i++)
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

        string  ColonFind = Colon(position);
        FindLongestLength(ColonFind, longestLength);

        string ColonDashFind = ColonDash(position);
        FindLongestLength(ColonDashFind, longestLength);

        string MultiFind = Multiply(position);
        FindLongestLength(MultiFind, longestLength);

        string AddFind = Add(position);
        FindLongestLength(AddFind, longestLength);

        //schemes
        string SchemesFind = Schemes(position);
        FindLongestLength(SchemesFind, longestLength);

        //facts
        string FactsFind = Facts(position);
        FindLongestLength(FactsFind, longestLength);

        //rules
        string RulesFind = Rules(position);
        FindLongestLength(RulesFind, longestLength);

        //queries
        string QueriesFind = Queries(position);
        FindLongestLength(QueriesFind, longestLength);

        //ID
        string IDFind = ID(position);
        FindLongestLength(IDFind, longestLength);

        string StringFSMFind = StringFSM(position);
        FindLongestLength(StringFSMFind, longestLength);

        string CommentFind = Comment(position);
        FindLongestLength(CommentFind, longestLength);

        string UndefinedFind = Undefined(position);
        FindLongestLength(UndefinedFind, longestLength);

        //-----------------------------------------------------------------------------------------
        //TOKENIZING LOGIC & VECTOR STORAGE

        //put keyword first, so that they get priority

        string tokenString = "";

        string test = SimpleFSM(longestLength, tokenString, tokenList, lineNumber, position);
        string test2 = MiddleFSM(longestLength, tokenString, tokenList, lineNumber, position);

        if (test == BLANK && test2 == BLANK) //this statement adds 2 complexity! boo!
            ComplexFSM(longestLength, tokenString, tokenList, lineNumber, position);

        whiteSpace(position); //skips over whitespace
        NewLineDetect(position, lineNumber);

        if (*position == '\0') //THE EMPTY CHAR!!! EOF
            break;
    }

    tokenList.push_back("(EOF,\"\"," + to_string(lineNumber) + ")");

    //***********************************************************************************************************
    //OUTPUT TO FILE:


    Output(tokenList);
    return 0;
}


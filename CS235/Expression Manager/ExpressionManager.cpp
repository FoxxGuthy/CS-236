#include "ExpressionManager.h"

const bool TEST1 = false;
const bool TEST2 = false;
const bool TEST3 = false;
const bool TEST4 = false;
const int DIVIDEZERO = 0;
const string INVALID = "invalid";


ExpressionManager::ExpressionManager()
{
}


ExpressionManager::~ExpressionManager()
{
}

bool ExpressionManager::isBalanced(string expression)
{
	if (TEST1)
		cout << "begin balanced" << endl << expression << endl;;

	stack <char> parantheses;
	stringstream ss(expression);
	char symbol;
	int loopCount = 0;
	while (!ss.eof())
	{
		ss >> symbol;
		/**
		we take in the first object, check if open, if so push it back
		if not, see if its a closer, if it is, check it against type of one in stack, if nots not, return false
		otherwise ignore



		*/
		if (isOpen(symbol))
			parantheses.push(symbol);
		else if (isClose(symbol))
		{
			if (TEST1) cout << "begin check for closing" << endl;
			if (symbol == ')')
			{
				if (TEST1) cout << "begin check for (" << endl;
				if (!checkP(parantheses, '('))
				{
					if (TEST1) cout << "was not balanced" << endl;
					return false;
				}
			}
				

			else if (symbol == ']')
			{
				if (TEST1) cout << "begin check for [" << endl;
				if (!checkP(parantheses, '['))
				{
					if (TEST1) cout << "was not balanced" << endl;
					return false;
				}
			}
				

			else if (symbol == '}')
			{
				if (TEST1) cout << "begin check for {" << endl;
				if (!checkP(parantheses, '{'))
				{

					return false;
				}
			}		
		}
		symbol = ' ';
		loopCount++;
	}


	if (parantheses.empty())
		return true;
	else
		return false;
}

string ExpressionManager::postfixToInfix(string expression)
{
	if (TEST2) cout << "begin post 2 in" << endl;
	stack <string> numbers;
	stringstream ss(expression);
	string symbol;

	while (!ss.eof())
	{
		ss >> symbol;

		if (isInt(symbol))
		{
			if (TEST2) cout << "pushing back int" << endl;
			numbers.push(symbol);
		}
		else if (isOperator(symbol))
		{
			if (TEST2) cout << "found operator" << endl;
			if (numbers.size() > 1)
			{
				symbol = symbol + " " + numbers.top() + " )";
				numbers.pop();
				symbol = "( " + numbers.top() + " " + symbol;
				numbers.pop();
				numbers.push(symbol);
			}
			else
			{
				//cout << "Not enough items in stack" << endl;
				return INVALID;
			}
		}	
	}
	if (numbers.size() == 1)
		return numbers.top();
	else
		return INVALID;
}

string ExpressionManager::infixToPostfix(string expression)
{
	if (TEST4) cout << "begin I to P " <<  expression << endl;
	stack <string> operators;
	stringstream ss(expression);
	string symbol;
	ostringstream output;

	int intCount = 0;
	int operatorCount = 0;

	if (!isBalanced(expression))
		return INVALID;

	while (!ss.eof())
	{
		ss >> symbol;

		if (isInt(symbol))
		{
			if (TEST4) cout << "is int " << symbol << endl;

			output << symbol << " ";
			intCount++;
		}
		else if (isOperator(symbol) || symbol == "(" || symbol == ")")
		{
			if (operators.empty())
			{
				//cout << "pushed back " << symbol << endl;
				operators.push(symbol);
				if (symbol != "(")
					operatorCount++;
			}

			else if (symbol == "(") // evaluate this one first, because we always want to push these back. 
			{
				operators.push(symbol);
				//cout << "pushed back " << symbol << endl;
			}

			else if (symbol == ")") //pop from stack until we find (
			{
				while (operators.top() != "(")
				{
					output << operators.top() << " "; // potentially we have space at end of a string which could be counted incorrect...
					//cout << "put operator in output " << operators.top() << endl;
					operators.pop();
				}
				operators.pop(); // this will delete the ( from the stack so nothing gets trapped under it. 
			}

			else if (getPrecedence(symbol) > getPrecedence(operators.top())) // greater precedence
			{
				operators.push(symbol);
				operatorCount++;
				//cout << "pushed back " << symbol << endl;
			}
				

			else if (!(getPrecedence(symbol) > getPrecedence(operators.top()))) // not greater precedence
			{
				//cout << "put operator in output " << symbol << endl;

				output << operators.top() << " ";
				operators.pop();
				operatorCount++;
				operators.push(symbol);
			}
		}
		else
			return INVALID;

		symbol = "";
	}

	while (!operators.empty()) // sometimes it works out that some operators are left in the stack, tack them on the end
	{
		output << operators.top();
		if (operators.size() != 1)
			output << " ";
		operators.pop();
	}

	if (intCount > operatorCount)
		return output.str();

	
	else
	{
		//cout << intCount << "ints " << operatorCount << " operators" << endl;
		//if (TEST4) cout << "too many operators" << endl;
		//cout << output.str() << endl;
		return INVALID;
	}
}

string ExpressionManager::postfixEvaluate(string expression)
{
	if (TEST3) cout << "begin post 2 in" << endl;
	stack <int> numbers;
	stringstream ss(expression);
	string symbol;

	while (!ss.eof())
	{
		ss >> symbol;

		if (isInt(symbol))
		{
			if (TEST3) cout << "pushing back int" << endl;
			/**
			will convert the string into an int and push back
			*/
			numbers.push(stoi(symbol));
		}
		else if (isOperator(symbol))
		{
			if (TEST3) cout << "found operator" << endl;
			int second = 0;
			int first = 0;
			if (numbers.size() > 1)
			{
				/**
				is there a way to make this a function, that will change operators based on what symbol its fed?
				I do the same thing 5 times.... is there a better way?
				*/

				second = numbers.top();
				numbers.pop();
				first = numbers.top();
				numbers.pop();

				if (symbol == "+")
				{
					first = first + second;
				}

				if (symbol == "-")
				{
					first = first - second;
				}
				if (symbol == "*")
				{
					first = first * second;
				}
				if (symbol == "/")
				{
					// DON'T DIVIDE BY ZERO
					if (second == DIVIDEZERO)
					{
						return INVALID;
					}
					first = first / second;
				}
				if (symbol == "%")
				{
					// DON'T DIVIDE BY ZERO
					if (second == DIVIDEZERO)
					{
						return INVALID;
					}
					first = first % second;
				}

				numbers.push(first);
			}
			else
			{
				//cout << "Not enough items in stack" << endl;
				return INVALID;
			}
		}
		/**
		if nots not an int or operator e.g. !@#$^ or {([
		*/
		else
			return INVALID;
	}
	if (numbers.size() == 1)
	{
		/**
		this simple code takes the int and converts to string
		*/
		string result;
		ostringstream convert;
		convert << numbers.top();
		result = convert.str();
		return result;
	}	
	else
		return INVALID;
}


/**
Used by isBalanced to check the parantheses
*/
bool const ExpressionManager::isOpen(char par)
{
	if (TEST1)
		cout << "begin isOpen" << endl;

	string const OPENING = "([{";

	// searches the string opening for the parantheseses.
	// what does the npos do?
	return OPENING.find(par) != string::npos;
}
/**
Used by isBalanced to check the parantheses
*/
bool const ExpressionManager::isClose(char par)
{
	if (TEST1)
		cout << "begin isClose" << endl;

	string const CLOSING = ")]}";
	return CLOSING.find(par) != string::npos;
}
/**
Used by isBalanced to check the closing p to top of stack
*/
bool  ExpressionManager::checkP(stack <char>& stuff, char open)
{
	/**
	if the first item is a closer, then the stack is empty, so top is an error
	*/
	if (stuff.empty())
		return false;
	if (stuff.top() == open)
	{
		stuff.pop();
		return true;
	}
	else
		return false;

}

/**
Used by postfix to infix. must catch a double, but can't crash if its nonsense e.g. symbols or combos (a3)
this will iterate thru each value of the string and determine if its a digit. will reject symbols
solves double issue by catching the "."
*/
bool const ExpressionManager::isInt(string number)
{
	bool isInt = true;
	for (unsigned i = 0; i < number.length(); i++)
	{
		if (!isdigit(number[i]))
			return false;
	}
	return isInt;
}

/**
Used by postfix to infix. same method as isOpen and isClose
*/
bool const ExpressionManager::isOperator(string symbol)
{
	string const OPERATOR = "+-*/%";
	return OPERATOR.find(symbol) != string::npos;
}

/**
used by infix to post. this will determine whether functions will be put in stack or into string
*/
int const ExpressionManager::getPrecedence(string symbol)
{
	int const HIGHEST = 3;
	int const MID = 2;
	int const LOW = 1;
	int const INVALID_SYMBOL = -1;

	if (symbol == "*" || symbol == "/" || symbol == "%")
		return HIGHEST;

	else if (symbol == "+" || symbol == "-")
		return MID;

	else if (symbol == "(")
		return LOW;

	return INVALID_SYMBOL;
}

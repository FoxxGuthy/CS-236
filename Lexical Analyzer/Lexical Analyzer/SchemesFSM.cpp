#include "SchemesFSM.h"



SchemesFSM::SchemesFSM()
{
	currentState = S0;
	accepted = false;
	acceptLength = NOT_FOUND;
}


SchemesFSM::~SchemesFSM()
{
}

/*
@Param: a character parsed from the getline string, which came from input file
uses switch case to see what letter was put in. If wasn't any matches, just ignores,
i decided to look at letters, could also have looked at State, and checked the letters
*/
bool SchemesFSM::transition(char x)
{
	switch (x) {
	case 'S':
		if (currentState == S0)
		{
			currentState = S1;
			acceptLength++;
			return true;
		}
		accepted = false;
		return false;
		break;

	case 'c':
		if (currentState == S1)
		{
			currentState = S2;
			acceptLength++;
			return true;
		}
		accepted = false;
		return false;
		break;

	case 'h':
		if (currentState == S2)
		{
			currentState = S3;
			acceptLength++;
			return true;
		}
		accepted = false;
		return false;
		break;

	case 'e':
		if (currentState == S3)
		{
			currentState = S4;
			acceptLength++;
			return true;
		}
			
		else if (currentState == S5)
		{
			currentState = S6;
			acceptLength++;
			return true;
		}
		accepted = false;
		return false;
		break;

	case 'm':
		if (currentState == S4)
		{
			currentState = S5;
			acceptLength++;
			return true;
		}
		accepted = false;
		return false;
		break;

	case 's':
		if (currentState == S6)
		{
			currentState = S7;
			accepted = true;
			return true;
		}
		accepted = false;
		return false;
		break;

	default:
		accepted = false;
		acceptLength = NOT_FOUND;
		return false;
	}
}

bool SchemesFSM::atFinalState()
{
	if (currentState == S7)
		return true;
	else
		return false;
}

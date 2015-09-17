#pragma once

const int FOUND = 1;
const int NOT_FOUND = 0;

class SchemesFSM
{
public:
	SchemesFSM();
	~SchemesFSM();

	bool transition(char x);
	bool atFinalState();

private:
	// There are 8 states, S0 is the start, and then one for each letter S-c-h-e-m-e-s
	enum STATE { S0, S1, S2, S3, S4, S5, S6, S7 };

	STATE currentState;

	//this will flag if we have reached the final state S7, meaning it was indeed a scheme
	bool accepted;

	//will say how long the length of what ever was accepted. Mostly for comparison against IDs which could be called something
	//with 'Schemes in the title
	unsigned acceptLength;
};

//need a function to return s7 to s0, perhaps a get token function that returns what whill be saved in the vector/output and takes line number as an input

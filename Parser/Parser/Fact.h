#pragma once
#include <vector>

class Fact
{
public:
	Fact();
	~Fact();

	string toString();

	string factID;
	vector<string> factStringVector;
};


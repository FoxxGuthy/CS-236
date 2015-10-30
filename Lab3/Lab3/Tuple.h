

#pragma once
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

class Tuple : public vector<string>
{
public:
	Tuple();
	virtual ~Tuple();

	void printTuple(stringstream&, vector<string>&, vector<size_t>&);
};


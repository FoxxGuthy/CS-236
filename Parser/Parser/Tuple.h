#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class Tuple: public vector<string>
{

public:
	void printTuple(stringstream& out, vector<string>& myAttributes, vector<size_t>& varIndex);
};


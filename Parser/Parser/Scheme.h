#pragma once
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class Scheme
{
public:
	Scheme();
	~Scheme();

	string toString();

	vector<string> IDvec;
	//scheme follows the following format:
	// ID ( ID, ID, ID, ID....) so we can just store the
};


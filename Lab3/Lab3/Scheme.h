

#pragma once
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

class Scheme {
public:
	Scheme();
	virtual ~Scheme();

	vector<string> myAttributes;

	void setAttributes(vector<string>&);
	vector<string>& getMyAttributes();

};


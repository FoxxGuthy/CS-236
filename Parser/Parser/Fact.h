#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <list>

using namespace std;

class Fact
{
public:
	Fact();
	~Fact();

	//FORMAT:
	//  ID ( string, string, string).
	string toString();

	//the first value is the factID, not part of the domain
	vector<string> factStringVector;

	//factStringList is only updated when this is called, so it will update and then
	list<string> getList();

private:
	list<string> factStringList;
};


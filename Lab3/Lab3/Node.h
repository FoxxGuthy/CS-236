#pragma once
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <string>

using namespace std;

class Node {
public:
	Node();//(set<string>, bool, int);
	virtual ~Node();

	set<string> adjacentNodes;
	bool visited;
	int postOrder;
};
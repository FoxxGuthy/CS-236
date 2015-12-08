

#include "Database.h"

using namespace std;

Database::Database()
{
}

Database::~Database()
{
}

void Database::addRelation(Relation r)
{
	myRelations.push_back(r);
}



//copy the datalog parse into our database
void Database::fill(vector<Predicate>& s, vector<Predicate>& f, vector<Predicate>& q, vector<Rule>& rules)
{
	mySchemes = s;
	myFacts = f;
	myQueries = q;
	myRules = rules;
	size_t i = 0;
	size_t j = 0;

	while (i < mySchemes.size())
	{
		Relation r;
		r.setNameSchema(s, i);

		for (j = 0; j < myFacts.size(); j++)
		{
			if (r.name == myFacts.at(j).info)
			{
				r.setTuples(f, j);
			}
		}

		addRelation(r);
		i++;
	}

	useRules(rules);

	evalQueries(q, false);

}

//evaluate the queries
void Database::evalQueries(vector<Predicate>& q, bool addNewFacts)
{
	size_t i = 0;
	Relation temp, relToAdd;

	while (i < q.size())
	{
		Relation r;
		r.setName(q, i);

		for (size_t j = 0; j < myRelations.size(); j++)
		{
			if (r.name == myRelations.at(j).name)
			{
				temp = myRelations.at(j);
				break;
			}
		}
		if (!addNewFacts) //regular
		{
			relToAdd = r.evalParams(q, i, temp);
			toPrint.push_back(relToAdd);
			i++;
		}
		else //for use with rules
		{
			relToAdd = r.evalParams(q, i, temp);
			newFacts.push_back(relToAdd);
			i++;
		}

	}
}

void Database::printResults(stringstream& out)
{
	vector<size_t> varIndex;
	vector<string> variablesAlreadytoBePrinted;
	bool toAdd = false;
	size_t numOfStringsDetected = 0;

	//out << "Schemes populated after " << numPasses << " passes through the Rules." << endl;
	out << "Rule Evaluation" << endl << numPasses << " passes: " << endl << endl;

	out << "Query Evaluation" << endl;
	for (size_t i = 0; i < myQueries.size(); i++)
	{
		out << myQueries.at(i).toString();

		printParams(varIndex, variablesAlreadytoBePrinted, toAdd, numOfStringsDetected, i, out);

		out << ")? ";

		toPrint.at(i).printRelation(i, out, varIndex);
		varIndex.clear();
		variablesAlreadytoBePrinted.clear();
		numOfStringsDetected = 0;
	}
}

void Database::printParams(vector<size_t>& varIndex, vector<string>& variablesAlreadytoBePrinted,
	bool toAdd, size_t numOfStringsDetected, size_t i, stringstream& out) {

	for (size_t j = 0; j < myQueries.at(i).paramList.size(); j++)
	{
		out << myQueries.at(i).paramList.at(j).toString();

		if (j < (myQueries.at(i).paramList.size() - 1))
		{
			out << ",";
		}

		if (myQueries.at(i).paramList.at(j).type == ID)
		{
			toAdd = true;

			if (varIndex.size() < 1)
			{
				varIndex.push_back(j - numOfStringsDetected);
				variablesAlreadytoBePrinted.push_back(myQueries.at(i).paramList.at(j).info);
			}
			else
			{
				for (size_t y = 0; y < variablesAlreadytoBePrinted.size(); y++)
				{
					if (myQueries.at(i).paramList.at(j).info == variablesAlreadytoBePrinted.at(y))
					{
						toAdd = false;
						break;
					}
				}
				if (toAdd == true)
				{
					varIndex.push_back(j - numOfStringsDetected);
					variablesAlreadytoBePrinted.push_back(myQueries.at(i).paramList.at(j).info);
				}
			}
		}
		else
		{
			numOfStringsDetected++;
		}
	}
}

Relation Database::find(string & n)
{
	Relation temp;

	for (size_t i = 0; i < myRelations.size(); i++)
	{
		if (myRelations.at(i).name == n)
		{
			temp = myRelations.at(i);
			myRelations.erase(myRelations.begin() + i);
			return temp;
		}
	}

	return temp;
}

int Database::getTupleCount()
{
	int size = 0;
	for (size_t i = 0; i < myRelations.size(); i++)
	{
		size += myRelations.at(i).getNumTuples();
	}
	return size;
}

void Database::useRules(vector<Rule>& rules)
{
	int oldSize, newSize, passCount = 0;
	bool tuplesAdded = true; //the size changed, so keep going

	while (tuplesAdded) //continue to evaluate while thigns have changed
	{
		oldSize = getTupleCount();
		evalRules(rules); //loops thru rules, evaluating
		passCount++;
		newSize = getTupleCount();
		if (oldSize != newSize)
		{
			tuplesAdded = true;
		}
		else //size didn't change, so the new additions didn't change anything. stop. 
		{
			tuplesAdded = false;
		}
	}


}

void Database::evalRules(vector<Rule>& rules)
{
	Relation temp1, temp2, temp3, temp4, clear;
	Relation projected, joined, toJoin;

	for (size_t i = 0; i < myRules.size(); i++)
	{
		temp1 = temp2 = temp3 = temp4 = clear;
		newFacts.clear();

		evalQueries(myRules.at(i).predList, true); //look for rules relating to the queries

		checkDuplicateSchemes(newFacts);

		if (newFacts.size() < 1) // no new facts
		{
			//do nothing
		}
		else if (newFacts.size() == 1) //only one. The JOIN!
		{
			projected = newFacts[0].project(myRules[i].myPred.paramList);
			toJoin = find(myRules[i].myPred.info);
			joined = projected.unionWith(toJoin);
			myRelations.push_back(joined);
		}
		else //lots of facts
		{
			evalRulesMultiFacts(temp1, temp2, temp3, projected, joined, toJoin, i);
		}
	}
}

void Database::evalRulesMultiFacts(Relation & one, Relation & two, Relation & three, Relation & projected, Relation & joined, Relation & toJoin, size_t i)
{
	for (size_t j = 0; j < newFacts.size(); j++)
	{
		Relation tempRel1 = newFacts[j];
		if (one.myTuples.size() == 0)
		{
			if (j < newFacts.size() - 1) // the last index
			{
				j++;
				Relation  tempRel2 = newFacts[j];

				one = one.join(tempRel1, tempRel2);

				if (j == newFacts.size() - 1)
				{
					projected = one.project(myRules[i].myPred.paramList);
					toJoin = find(myRules[i].myPred.info);
					joined = projected.unionWith(toJoin);
					myRelations.push_back(joined);

				}
			}
			else
			{
				two = two.join(one, tempRel1);
				projected = two.project(myRules[i].myPred.paramList);
				toJoin = find(myRules[i].myPred.info);
				joined = projected.unionWith(toJoin);
				myRelations.push_back(joined);
			}
		}
		else
		{
			three = three.join(one, tempRel1);
			one = three;
			if (j == newFacts.size() - 1)
			{
				projected = one.project(myRules[i].myPred.paramList);
				toJoin = find(myRules[i].myPred.info);
				joined = projected.unionWith(toJoin);
				myRelations.push_back(joined);
			}
		}
	}
}

void Database::checkDuplicateSchemes(vector<Relation>& r)
{
	string currAttribute;

	for (size_t i = 0; i < r.size(); i++)
	{
		Scheme newScheme;
		Relation curr = r.at(i);
		Scheme currScheme = r.at(i).scheme;

		if (currScheme.myAttributes.size() > 1)
		{
			for (size_t j = 0; j < currScheme.myAttributes.size(); j++)
			{
				currAttribute = currScheme.myAttributes.at(j);

				if (contains(currAttribute, newScheme.myAttributes) == false)
				{
					newScheme.myAttributes.push_back(currAttribute);
				}
			}
			r.at(i).scheme = newScheme;
		}
	}
}

bool Database::contains(string & curr, vector<string>& currAttributes)
{
	for (size_t i = 0; i < currAttributes.size(); i++)
	{
		string temp = currAttributes.at(i);
		if (temp == curr)
		{
			return true;
		}
	}
	return false;
}

void Database::removeDupSchemes(Relation & r, vector<size_t>& removeIndex)
{
	Scheme newScheme;
	bool toRemove;

	for (size_t i = 0; i < r.scheme.myAttributes.size(); i++)
	{
		toRemove = false;

		for (size_t j = 0; j < removeIndex.size(); j++)
		{
			if (removeIndex.at(j) == i)
			{
				toRemove = true;
				break;
			}
		}
		if (!toRemove)
		{
			newScheme.myAttributes.push_back(r.scheme.myAttributes.at(i));
		}
	}
	r.scheme = newScheme;
}

//lab 5

void Database::createGraph(vector<Predicate>& myQueries, vector<Rule>& myRules)
{
	for (size_t i = 0; i < myQueries.size(); i++)
	{
		string currQ = myQueries.at(i).info;
		Node n;
		int curr = i;
		//string key = "Q" + convertInt(curr);
		set<string> currSet;

		for (size_t j = 0; j < myRules.size(); j++)
		{
			string currR = myRules.at(j).myPred.info;
			string key2;
			int curr = j + 1;

			if (currQ == currR)
			{
				key2 = "R" + convertInt(curr);
				currSet.insert(key2);
			}
		}
		n.adjacentNodes = currSet;
		//myGraph[key] = n;
	}

	for (size_t i = 0; i < myRules.size(); i++)
	{
		string currR = myRules.at(i).myPred.info;
		Node n;
		int curr = i + 1;
		string key = "R" + convertInt(curr);
		set<string> currSet;

		for (size_t z = 0; z < myRules.at(i).predList.size(); z++)
		{
			string currPred = myRules.at(i).predList.at(z).info;
			string key2;

			for (size_t j = 0; j < myRules.size(); j++)
			{
				int curr = j + 1;
				string ruleHead = myRules.at(j).myPred.info;

				if (currPred == ruleHead)
				{
					key2 = "R" + convertInt(curr);
					currSet.insert(key2);
				}
			}
		}

		n.adjacentNodes = currSet;
		myGraph[key] = n;
	}
}

void Database::printGraph(stringstream& out)
{
	map<string, Node>::iterator it;
	set<string>::iterator iter;

	out << "Dependency Graph" << endl;

	for (it = myGraph.begin(); it != myGraph.end(); it++)
	{
		out << "  " << it->first << ":";

		Node n = it->second;
		set<string> curr = n.adjacentNodes;
		for (iter = curr.begin(); iter != curr.end(); iter++)
		{
			string curr = *iter;
			out << " " << curr;
		}
		out << endl << endl;
	}
}

void Database::printQuery(stringstream& out, vector<Predicate>& myQueries)
{
	//out << endl;
	for (size_t i = 0; i < myQueries.size(); i++)
	{
		/*out << myQueries.at(i).toString();*/

		//for (size_t j = 0; j < myQueries.at(i).paramList.size(); j++)
		//{
		//	out << myQueries.at(i).paramList.at(j).toString();

		//	if (j < (myQueries.at(i).paramList.size() - 1))
		//	{
		//		out << ",";
		//	}
		//}
		//out << ")? " << endl;
		//out << endl;

		string currQ = "Q" + convertInt(i + 1);
		depthFirst(currQ);

		//printPostNums(out);
		printRuleOrder(out, currQ);
		//printBackwardEdges(out);
		reset();
	}
}

void Database::depthFirst(string& currQ)
{
	set<string>::iterator iter;

	Node q = myGraph[currQ];
	set<string> currAdj = q.adjacentNodes;
	myGraph[currQ].visited = true;

	if (!currAdj.empty())
	{
		for (iter = currAdj.begin(); iter != currAdj.end(); iter++)
		{
			string dependsOn = *iter;
			if (dependsOn != "" && myGraph[dependsOn].visited == false)
			{
				depthFirst(dependsOn);
			}
		}
		myGraph[currQ].postOrder = postCntr;
		postCntr++;
	}
	else
	{
		myGraph[currQ].postOrder = postCntr;
		postCntr++;
	}
}

void Database::printPostNums(stringstream& out)
{
	map<string, Node>::iterator it;

	//out << "  Postorder Numbers" << endl;

	for (it = myGraph.begin(); it != myGraph.end(); it++)
	{
		Node n = it->second;
		if (n.postOrder != 3000000)
		{
			//out << "    " << it->first << ": " << n.postOrder << endl;
		}
	}

	//out << endl;
}

void Database::printRuleOrder(stringstream& out, string currQ)
{
	map<string, Node>::iterator it;
	int endCount = myGraph[currQ].postOrder;
	int count = 1;

	//out << "  Rule Evaluation Order" << endl;

	while (count < endCount)
	{
		for (it = myGraph.begin(); it != myGraph.end(); it++)
		{
			string curr = it->first;
			Node n = it->second;

			if (curr[0] == 'Q')
			{

			}
			else
			{
				if (n.postOrder == count)
				{
					out << "    " << curr << endl;
					count++;
				}
			}
		}
	}

	//out << endl;
}

//void Database::printBackwardEdges(stringstream& out)
//{
//	out << "  Backward Edges" << endl;
//
//	map<string, Node>::iterator it;
//
//	for (it = myGraph.begin(); it != myGraph.end(); it++)
//	{
//		string curr = it->first;
//		Node n = it->second;
//		int currPostOrder = n.postOrder;
//
//		if (curr[0] == 'R')
//		{
//			continuePrintingBackwardEdges(curr, n, currPostOrder, out);
//		}
//	}
//
//	out << endl;
//}

void Database::continuePrintingBackwardEdges(string& curr, Node& n, int& currPostOrder, stringstream& out) {
	int counter = 1;
	bool startedPrinting = false;
	bool closedLine = false;
	set<string>::iterator iter;

	for (iter = n.adjacentNodes.begin(); iter != n.adjacentNodes.end(); iter++)
	{
		string adjacent = *iter;
		if (myGraph[adjacent].postOrder != 3000000)
		{
			int adjPostOrder = myGraph[adjacent].postOrder;

			if (currPostOrder <= adjPostOrder)
			{
				if (startedPrinting == false)
				{
					if (counter == n.adjacentNodes.size())
					{
						out << "    " << curr << ": " << adjacent << endl;
						closedLine = true;
						startedPrinting = true;
					}
					else
					{
						out << "    " << curr << ": " << adjacent;
						closedLine = false;
						startedPrinting = true;
					}
				}
				else
				{
					out << " " << adjacent;
				}
			}
		}
		counter++;
	}
	if (startedPrinting == true && closedLine == false)
	{
		out << endl;
	}
}

void Database::reset()
{
	map<string, Node>::iterator it;

	for (it = myGraph.begin(); it != myGraph.end(); it++)
	{
		string curr = it->first;
		myGraph[curr].postOrder = 3000000;
		myGraph[curr].visited = false;
	}

	postCntr = 1;
}

string Database::convertInt(int number)
{
	stringstream ss;
	ss << number;
	return ss.str();
}
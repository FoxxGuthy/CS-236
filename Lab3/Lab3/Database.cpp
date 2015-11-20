

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

	out << "Schemes populated after " << numPasses << " passes through the Rules." << endl;

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


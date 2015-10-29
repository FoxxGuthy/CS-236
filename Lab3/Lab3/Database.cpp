

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
void Database::fill(vector<Predicate>& s,vector<Predicate>& f,vector<Predicate>& q)
{
	mySchemes = s;
	myFacts = f;
	myQueries = q;
	size_t i = 0;
	size_t j = 0;

	while(i < mySchemes.size())
	{
		Relation r;
		r.setNameSchema(s, i);

		for(j = 0; j < myFacts.size(); j++)
		{
			if(r.name == myFacts.at(j).info)
			{
				r.setTuples(f,j);
			}
		}

		addRelation(r);
		i++;
	}

	evalQueries(q);

}

//evaluate the queries
void Database::evalQueries(vector<Predicate>& q)
{
	size_t i = 0;
	Relation temp, relToAdd;

	while(i < myQueries.size())
	{
		Relation r;
		r.setName(q, i);

		for(size_t j = 0; j < myRelations.size(); j++)
		{
			if(r.name == myRelations.at(j).name)
			{
				temp = myRelations.at(j);
			}
		}

		relToAdd = r.evalParams(q,i,temp);
		toPrint.push_back(relToAdd);
		i++;

	}
}

void Database::printResults(stringstream& out)
{
	vector<size_t> varIndex;
	vector<string> variablesAlreadytoBePrinted;
	bool toAdd = false;

	for(size_t i = 0; i < myQueries.size(); i++)
	{
		out << myQueries.at(i).toString();

		printParams(varIndex, variablesAlreadytoBePrinted, toAdd, i, out);

		out << ")? ";

		toPrint.at(i).printRelation(i, out, varIndex);
		varIndex.clear();
		variablesAlreadytoBePrinted.clear();
	}
}

void Database::printParams(vector<size_t>& varIndex, vector<string>& variablesAlreadytoBePrinted, bool toAdd, size_t i, stringstream& out) {

	for(size_t j = 0; j < myQueries.at(i).paramList.size(); j++)
	{
		out << myQueries.at(i).paramList.at(j).toString();

		if(j < (myQueries.at(i).paramList.size() - 1))
		{
			out << ",";
		}

		if(myQueries.at(i).paramList.at(j).type == ID)
		{
			toAdd = true;

			if(varIndex.size() < 1)
			{
				varIndex.push_back(j);
				variablesAlreadytoBePrinted.push_back(myQueries.at(i).paramList.at(j).info);
			}
			else
			{
				for(size_t y = 0; y < variablesAlreadytoBePrinted.size(); y++)
				{
					if(myQueries.at(i).paramList.at(j).info == variablesAlreadytoBePrinted.at(y))
					{
						toAdd = false;
						break;
					}
				}
				if(toAdd == true)
				{
					varIndex.push_back(j);
					variablesAlreadytoBePrinted.push_back(myQueries.at(i).paramList.at(j).info);
				}
			}
		}
	}
}


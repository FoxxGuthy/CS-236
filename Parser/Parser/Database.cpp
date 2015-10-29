#include "Database.h"



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

void Database::fillRelations()
{
	for (size_t i = 0; i < DP.schemeVector.size(); i++) //make relation for each scheme
	{
		Relation R;
		R.setSchema(DP.schemeVector, i);

		for (size_t j = 0; j < DP.factVector.size(); j++)
		{
			if (R.name == DP.factVector[j].factStringVector[0]) //the first index is the factID
			{
				R.setTuples(DP.factVector, j);
			}
		}
		addRelation(R);
	}
	evalQueries();
}

void Database::evalQueries()
{
	Relation temp, reltoAdd;

	for (size_t i = 0; i < DP.queryVector.size(); i++)
	{
		Relation R;
		R.name = DP.queryVector[i].queryPred[0].PredIDsVec[0]; //probably bug here, that name may need to be all of PredIDsVec

		for (size_t j = 0; j < myRelations.size(); j++)
		{
			if (R.name == myRelations[j].name)
			{
				temp = myRelations[j];
			}
		}

		reltoAdd = R.evalParams(DP.queryVector, i, temp);
		toPrint.push_back(reltoAdd);
	}
}

void Database::printQueryResults(stringstream& out)
{
	vector<size_t> varIndex;
	vector<string> variablesToPrint;
	bool toAdd = false;

	for (size_t i = 0; i < DP.queryVector.size(); i++)
	{
		out << DP.queryVector[i].toString();

		printParams(varIndex, variablesToPrint, toAdd, i, out);

		out << ")QMARK";
		
		toPrint[i].PrintRelation(i, out, varIndex);
		varIndex.clear();
		variablesToPrint.clear();
	}
}

void Database::printParams(vector<size_t>& varIndex, vector<string>& variablestoBePrinted, bool toAdd, size_t i, stringstream & out)
{
	for (size_t j = 0; j < DP.queryVector[i].queryPred.size(); j++)
	{
		out << DP.queryVector[i].queryPred[j].toString();

		if (j < DP.queryVector[i].queryPred.size() - 1)
		{
			out << "CHEESE" << endl;
		}

		if (!DP.queryVector[i].queryPred[j].PredIDsVec.empty()) //its not a headPred, just list of paramaters
		{
			toAdd = true;

			if (varIndex.size() < 1)
			{
				varIndex.push_back(j);
				variablestoBePrinted.push_back(DP.queryVector[i].queryPred[j].toString()); //print out IDvect

			}
			else
			{
				for (size_t y = 0; y < variablestoBePrinted.size(); y++)
				{
					if (DP.queryVector[i].queryPred[j].toString() == variablestoBePrinted[y])
					{
						toAdd = false;
						break;
					}
				}
				if (toAdd)
				{
					varIndex.push_back(j);
					variablestoBePrinted.push_back(DP.queryVector[i].queryPred[j].toString());
				}
			}
		}
	}
}


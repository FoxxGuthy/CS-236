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
	}
}

string Database::printQueryResults()
{
	return "";
}

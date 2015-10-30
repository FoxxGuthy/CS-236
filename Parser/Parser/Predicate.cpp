#include "Predicate.h"



Predicate::Predicate()
{
}


Predicate::~Predicate()
{
}

string Predicate::toString()
{
	stringstream ss;
	if (!PredIDsVec.empty()) //this means its a headPred... I think
	{
		ss << PredIDsVec[0] << "(";
		for (int i = 1; i < PredIDsVec.size(); i++)
		{
			ss << PredIDsVec[i];
			if (PredIDsVec[i] != PredIDsVec.back())
			{
				ss << ",";
			}	
		}
		//ss << ")";
	}
	if (!PredParmVec.empty())
	{
		ss << PredParmVec[0].toString() << ",";
		for (int j = 1; j < PredParmVec.size(); j++)
		{
			ss << PredParmVec[j].toString();
			if (PredParmVec[j].toString() != PredParmVec.back().toString())
			{
				ss << ",";
			}
		}
		ss << ")";
	}
	return ss.str();
}

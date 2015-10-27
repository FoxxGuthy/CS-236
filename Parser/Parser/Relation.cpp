#include "Relation.h"



Relation::Relation()
{
}


Relation::~Relation()
{
}

void Relation::setSchema(vector<Scheme>& s, size_t index)
{
	name = s[index].IDvec[0];

	for (size_t j = 0; j < s[index].IDvec.size(); j++)
	{
		rScheme.IDvec.push_back(s[index].IDvec[j]);
	}
}

void Relation::setTuples(vector<Fact>& f, size_t index)
{
	Tuple myTuple;
	for (size_t l = 0; l < f[index].factStringVector.size(); l++)
	{
		myTuple.push_back(f[index].factStringVector[l]);

	}
	Tuples.insert(myTuple);
}

Relation Relation::evalParams(vector<Query>& q, size_t i, Relation source)
{
	return Relation();
}

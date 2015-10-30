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
	Relation two, three;
	vector<int> toProject;
	vector<size_t> duplicateParams;
	vector<string> ids;
	size_t duplicateIndex;
	vector<string> alreadyProjected;
	bool toAdd;

	for (size_t j = 0; j < q[i].queryPred.size(); j++)
	{
		toAdd = true;

		if (q[i].queryPred.empty()) //its not a head pred, just a list of params
		{
			if (two.name == "")
			{
				two = source.select(j, q[i].queryPred[j].toString());
			}
			else
			{
				two = two.select(j, q[i].queryPred[j].toString());
			}
		}
		else
		{
			//evalParamID
		}


		//NOT COMPLETEFINISH 
	}
}

Relation Relation::select(size_t index, string value)
{
	Relation r;
	r.name = name; //give this new relation the same as this' name and scheme
	r.rScheme = rScheme;
	set<Tuple>::iterator it;
	for (it = Tuples.begin(); it != Tuples.end(); it++)
	{
		Tuple temp;
		temp = *it;

		if (temp.at(index) == value)
		{
			r.Tuples.insert(temp);
		}
	}
	return r;
}

Relation Relation::select(size_t index1, size_t index2)
{
	Relation r;
	r.name = name; //give this new relation the same as this' name and scheme
	r.rScheme = rScheme;
	set<Tuple>::iterator it;
	for (it = Tuples.begin(); it != Tuples.end(); it++)
	{
		Tuple temp;
		temp = *it;

		if (temp.at(index1) == temp.at(index2))
		{
			r.Tuples.insert(temp);
		}
	}
	return r;
}

Relation Relation::project(vector<int>& myIndexVector)
{
	Relation r;
	r.name = name;
	r.rScheme = rScheme;
	set<Tuple>::iterator it;
	Tuple projected;
	int toProject;

	for (it = Tuples.begin(); it != Tuples.end(); it++);
	{
		Tuple Temp;
		Temp = *it;
		for (size_t i = 0; i < myIndexVector.size(); i++)
		{
			toProject = myIndexVector.at(i);
			projected.push_back(Temp.at(toProject));
		}

		r.Tuples.insert(projected);
		projected.clear();
		
	}
	return r;
}

void Relation::PrintRelation(size_t i, stringstream & out, vector<size_t>& varIndex)
{
	set<Tuple>::iterator it;
	vector<string> myAttributes = rScheme.IDvec;

	if (Tuples.size() < 1) //empty relation
	{
		out << "No" << endl;
	}
	else
	{
		out << "Yes(" << Tuples.size() << ")" << endl;
	}

	for (it = Tuples.begin(); it != Tuples.end(); it++)
	{
		Tuple Temp;
		Temp = *it;

		Temp.printTuple(out, myAttributes, varIndex);
	}
}




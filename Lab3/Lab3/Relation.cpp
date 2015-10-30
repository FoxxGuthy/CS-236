

#include "Relation.h"

using namespace std;

Relation::Relation()
{
}

Relation::~Relation()
{
}


Relation Relation::select(size_t index, string value) //reduce rows to those that contain string value
{
	Relation r;
	r.name = name;
	r.scheme = scheme;
	set<Tuple>::iterator it;

	for(it = myTuples.begin(); it != myTuples.end(); it++)
	{
		Tuple temp;
		temp = *it;

		if(temp.at(index) == value)
		{
			r.myTuples.insert(temp);
		}
	}

	return r;
}

Relation Relation::select(size_t index1, size_t index2) //reduct rows to those that are at index2
{
	Relation r;
	r.name = name;
	r.scheme = scheme;
	set<Tuple>::iterator it;

	for(it = myTuples.begin(); it != myTuples.end(); it++)
	{
		Tuple temp;
		temp = *it;

		if(temp.at(index1) == temp.at(index2))
		{
			r.myTuples.insert(temp);
		}
	}

	return r;
}

Relation Relation::project(vector<int>& myIndicies) //reduce columns to those index we have
{
	Relation r;
	r.name = name;
	r.scheme = scheme;
	set<Tuple>::iterator it;
	Tuple projected;
	int toProject;

	for(it = myTuples.begin(); it != myTuples.end(); it++)
	{
		Tuple temp;
		temp = *it;

		for(size_t i = 0; i < myIndicies.size(); i++)
		{
			toProject = myIndicies.at(i);
			projected.push_back(temp.at(toProject));
		}

		r.myTuples.insert(projected);
		projected.clear();
	}

	return r;
}

void Relation::rename(string s, size_t j) //changes the name of a scheme in the Relation
{
	scheme.myAttributes.at(j) = s;
}

//sets the name attribute of the relation
//then gives it vector of parameters
void Relation::setNameSchema(vector<Predicate>& s, size_t i)
{
	mySchemes = s;
	size_t index = i;
	size_t j = 0;
	vector<string> schema;

	name = mySchemes.at(index).info;

	while(j < mySchemes.at(index).paramList.size())
	{
		scheme.myAttributes.push_back(mySchemes.at(index).paramList.at(j).info);
		j++;
	}
}

void Relation::setTuples(vector<Predicate>& facts, size_t i)  //passes in facts, gives the relation its tuples
{
	myFacts = facts;
	size_t index = i;
	size_t j = 0;
	Tuple t;

	while(j < myFacts.at(index).paramList.size())
	{
		t.push_back(myFacts.at(index).paramList.at(j).info);
		j++;
	}

	myTuples.insert(t);
}

void Relation::setName(vector<Predicate>& q, size_t i)// sets the name of the relation
{
	myQueries = q;
	size_t index = i;

	name = myQueries.at(index).info;
}

Relation Relation::evalParams(vector<Predicate>& q, size_t i, Relation source) //evaluate the queries
{
	size_t index = i;
	size_t j = 0;
	Relation two,three;
	vector<int> toProject; //index of columns we want to keep
	vector<size_t> duplicateParams; //possible duplicate parameters
	vector<string> ids;
	size_t duplicateIndex;
	vector<string> alreadyProjected;
	bool toAdd;

	while(j < myQueries.at(index).paramList.size())
	{
		toAdd = true;

		if(myQueries.at(index).paramList.at(j).type == STRING)
		{
			if(two.name == "") //if relation two hasn't been edited use the source
			{
				two = source.select(j,myQueries.at(index).paramList.at(j).info);
				j++;
			}
			else //relation two has been edited so continue to edit it
			{
				two = two.select(j,myQueries.at(index).paramList.at(j).info);
				j++;
			}
		}
		else //type == ID so project?
		{
			evalParamID(source, two,index, j, duplicateIndex,alreadyProjected, toProject, toAdd);
		}
	}//done evaluating a query

	if (toProject.size() > 0)
	{
		if (two.name == "") {
			three = source.project(toProject);
		}
		else {
			three = two.project(toProject);
		}
		return three;
	}
	else {
		return two;
	}
}

void Relation::evalParamID(Relation& source, Relation& two,
		size_t& index, size_t& j, size_t& duplicateIndex,
		vector<string>& alreadyProjected, vector<int>& toProject, bool toAdd) {

	source.rename(myQueries.at(index).paramList.at(j).info, j);
	if (two.name != "") 
	{
		two.rename(myQueries.at(index).paramList.at(j).info, j);
	}

	duplicateIndex = isThereAnother(myQueries.at(index).paramList.at(j).info,
			myQueries.at(index).paramList, j);

	if (duplicateIndex != -1)  //there was a duplicate
	{
		thereIsADuplicate(source, two, index, j, duplicateIndex, alreadyProjected, toProject, toAdd);
	}
	else 
	{
		for (size_t y = 0; y < alreadyProjected.size(); y++) {
			if (myQueries.at(index).paramList.at(j).info
					== alreadyProjected.at(y)) {
				toAdd = false;
				break;
			}
		}
		if (toAdd == true) {
			toProject.push_back(j);
		}
		j++;
	}
}

//handles duplicate parameters
void Relation::thereIsADuplicate(Relation& source, Relation& two,
		size_t& index, size_t& j, size_t& duplicateIndex,
		vector<string>& alreadyProjected, vector<int>& toProject, bool toAdd) {

	if (two.name == "") {
		two = source.select(j, duplicateIndex);
	} else {
		two = two.select(j, duplicateIndex);
	}

	for (size_t y = 0; y < alreadyProjected.size(); y++) 
	{
		if (myQueries.at(index).paramList.at(j).info //we've already projected, so do nothing
			== alreadyProjected.at(y)) {
			toAdd = false;
			break;
		}
	}
	if (toAdd == true) //not projected...
	{
		toProject.push_back(j);
		alreadyProjected.push_back(myQueries.at(index).paramList.at(j).info);
	}
	j++;
}

//returns index of a duplicate paramter
size_t Relation::isThereAnother(string id, vector<Parameter> p, size_t j)
{
	size_t duplicateIndex = -1;

	if(j+1 < p.size())
	{
		for(size_t i = j+1; i < p.size(); i++)
		{
			if(duplicateIndex == -1)
			{
				if(p.at(i).info == id)
				{
					duplicateIndex = i;
					break;
				}
			}
		}
	}
	else
	{
		duplicateIndex = -1;
	}

	return duplicateIndex;
}

void Relation::printRelation(size_t i, stringstream& out, vector<size_t>& varIndex)
{
	set<Tuple>::iterator it;
	vector<string> myAttributes = scheme.getMyAttributes();

	if(myTuples.size() < 1)//empty relation
	{
		out << "No" << endl;
	}
	else
	{
		out << "Yes(" << myTuples.size() << ")" << endl;
	}

	for(it = myTuples.begin(); it != myTuples.end(); it++)
	{
		Tuple temp;
		temp = *it;

		temp.printTuple(out, myAttributes, varIndex);
	}
}
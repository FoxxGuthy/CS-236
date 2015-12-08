

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

	Scheme s;

	for(it = myTuples.begin(); it != myTuples.end(); it++)
	{
		Tuple temp;
		temp = *it;

		for(size_t i = 0; i < myIndicies.size(); i++)
		{
			toProject = myIndicies.at(i);
			projected.push_back(temp.at(toProject));
			//CHANGE MADE BELOW		
			s.myAttributes.push_back(scheme.myAttributes.at(toProject));
		}

		r.myTuples.insert(projected);
		projected.clear();
	}
	//change made below		
	r.scheme = s;
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

Relation Relation::project(vector<Parameter>& paramList)
{
	Relation r;
	r.name = name;
	r.scheme = scheme;
	vector<int> myIndexList;

	for (size_t i = 0; i < paramList.size(); i++)
	{
		for (size_t j = 0; j < scheme.myAttributes.size(); j++)
		{
			if (paramList[i].info == scheme.myAttributes[j])
			{
				myIndexList.push_back(j);
			}
		}
	}

	r = project(myIndexList);
	return r;
}

Relation Relation::unionWith(Relation & toUnion)
{
	set<Tuple>::iterator it;

	for (it = myTuples.begin(); it != myTuples.end(); it++)
	{
		Tuple temp;
		temp = *it;

		toUnion.myTuples.insert(temp);
	}

	return toUnion;
}

Relation Relation::join(Relation & one, Relation & two)
{
	Relation r;
	Scheme s;
	Tuple t;
	set<Tuple>::iterator it;
	set<Tuple>::iterator iter;

	s = join(one.scheme, two.scheme);
	r.scheme = s;

	for (it = one.myTuples.begin(); it != one.myTuples.end(); it++)
	{
		Tuple t1 = *it;

		for (iter = two.myTuples.begin(); iter != two.myTuples.end(); iter++)
		{
			Tuple t2 = *iter;

			if (joinable(one.scheme, two.scheme, t1, t2) == true)
			{
				t = join(one.scheme, two.scheme, t1, t2);
				r.myTuples.insert(t);
			}
		}
	}

	return r;
}

Scheme Relation::join(Scheme s1, Scheme s2)
{
	Scheme s;
	bool toAdd;

	for (size_t i = 0; i < s2.myAttributes.size(); i++)
	{
		toAdd = true;

		for (size_t j = 0; j < s1.myAttributes.size(); j++)
		{
			if (s2.myAttributes.at(i) == s1.myAttributes.at(j))
			{
				toAdd = false;
				break;
			}
		}
		if (toAdd)
		{
			s1.myAttributes.push_back(s2.myAttributes.at(i));
		}
	}

	return s1;
}

Tuple Relation::join(Scheme &s1, Scheme &s2, Tuple &t1, Tuple &t2)
{
	Tuple t;
	bool toAdd;

	for (size_t i = 0; i < t1.size(); i++)
	{
		t.push_back(t1.at(i));
	}
	for (size_t j = 0; j < t2.size(); j++)
	{
		toAdd = true;

		for (size_t z = 0; z < s1.myAttributes.size(); z++)
		{
			if (s2.myAttributes.at(j) == s1.myAttributes.at(z))
			{
				toAdd = false;
				break;
			}
		}
		if (toAdd)
		{
			t.push_back(t2.at(j));
		}
	}
	return t;
}

bool Relation::joinable(Scheme & s1, Scheme & s2, Tuple & t1, Tuple & t2)
{
	string name1, name2;
	string v1, v2;

	for (size_t i = 0; i < s1.myAttributes.size(); i++)
	{
		name1 = s1.myAttributes.at(i);
		v1 = t1.at(i);

		for (size_t j = 0; j < s2.myAttributes.size(); j++)
		{
			name2 = s2.myAttributes.at(j);
			v2 = t2.at(j);

			if (name1 == name2 && v1 != v2)
			{
				return false;
			}
		}
	}
	return true;
}

int Relation::getNumTuples()
{
	int size = 0;
	set<Tuple>::iterator it;

	for (it = myTuples.begin(); it != myTuples.end(); it++)
	{
		Tuple temp;
		temp = *it;

		size += temp.size();
	}

	return size;
}

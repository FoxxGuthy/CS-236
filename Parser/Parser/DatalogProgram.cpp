#include "DatalogProgram.h"



DatalogProgram::DatalogProgram()
{
}


DatalogProgram::~DatalogProgram()
{
}

string DatalogProgram::schemeToString()
{
	stringstream ss;
	ss << "Schemes(" << schemeVector.size() << "):" << endl;
	for (auto sc : schemeVector)
	{
		ss << "  " << sc->toString() << endl;
	}
	return ss.str();
}

string DatalogProgram::factToString()
{
	stringstream ss;
	ss << "Facts(" << factVector.size() << "):" << endl;
	for (auto fa : factVector)
	{
		ss << "  " << fa->toString() << endl;
	}
	return ss.str();
}

void DatalogProgram::updateDomain()
{
	//go thru the Facts vector and get each one's string list, 
	//add each element to our over all list.
	//sort and remove duplicates
	for (int i = 0; i < factVector.size(); i++) //for each fact
	{
		//merge the lists
		domainList.merge(factVector[i]->getList());
	}

	//remove duplicates from the domain
	domainList.unique();
	//sort the list
	domainList.sort();
}

string DatalogProgram::ruleToString()
{
	stringstream ss;
	ss << "Rules(" << ruleVector.size() << "):" << endl;
	for (auto ru : ruleVector)
	{
		ss << "  " << ru->toString() << endl;
	}
	return ss.str();
}

string DatalogProgram::queryToString()
{
	stringstream ss;
	ss << "Queries(" << queryVector.size() << "):" << endl;
	for (auto qu : queryVector)
	{
		ss << "  " << qu->toString() << endl;
	}
	return ss.str();
}

string DatalogProgram::domainToString()
{
	updateDomain();
	//parse to a string
	stringstream ss;
	ss << "Domain(" << domainList.size() << "):" << endl;
	for (auto &domain : domainList)
	{
		ss << "  " << domain << endl;
	}
	return ss.str();
}

string DatalogProgram::ToString()
{
	stringstream returnString;
	returnString << "Success!" << endl;

	returnString << schemeToString() << factToString() << ruleToString() << queryToString() << domainToString();
	return returnString.str();
}

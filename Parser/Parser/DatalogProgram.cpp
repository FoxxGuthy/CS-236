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
	for (auto &sc : schemeVector)
	{
		ss << "  " << sc->toString() << endl;
	}
	return ss.str();
}

//string DatalogProgram::factToString()
//{
//	stringstream ss;
//	ss << "Facts(" << factVector.size() << "):" << endl;
//	for (auto &fa : factVector)
//	{
//		//ss << "  " << sc->ToString() << endl;
//	}
//	return ss.str();
//}
//
//string DatalogProgram::ruleToString()
//{
//	stringstream ss;
//	ss << "Schemes(" << ruleVector.size() << "):" << endl;
//	for (auto &ru : ruleVector)
//	{
//		//ss << "  " << sc->ToString() << endl;
//	}
//	return ss.str();
//}
//
//string DatalogProgram::queryToString()
//{
//	stringstream ss;
//	ss << "Schemes(" << queryVector.size() << "):" << endl;
//	for (auto &qu : queryVector)
//	{
//		//ss << "  " << sc->ToString() << endl;
//	}
//	return ss.str();
//}
//
//string DatalogProgram::domainToString()
//{
//	//remove duplicates from the domain
//	domainList.unique();
//	//sort the list
//	domainList.sort();
//
//	//parse to a string
//	stringstream ss;
//	ss << "Domain(" << domainList.size() << "):" << endl;
//	for (auto &domain : domainList)
//	{
//		ss << "  " << domain << endl;
//	}
//	return ss.str();
//}

string DatalogProgram::ToString()
{
	stringstream returnString;
	returnString << "Success!" << endl;

	
	returnString << schemeToString() /*<< factToString() << ruleToString() << queryToString() << domainToString()*/;
	return returnString.str();
}

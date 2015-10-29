

#include "Predicate.h"

using namespace std;

Predicate::Predicate()
{

}

Predicate::Predicate(string data)
{
	info = data;
}

Predicate::~Predicate() {
	
}

void Predicate::addParameter(Parameter p)
{
	paramList.push_back(p);
}

void Predicate::setInfo(string s)
{
	info = s;
}

string Predicate::toString()//stringstream & out)
{
	ostringstream sb;
	//sb << "  " << info << "(";  changed to one below for lab 4
	sb << info << "(";
	return sb.str();
}









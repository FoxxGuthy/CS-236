

#include "Parameter.h"

using namespace std;

Parameter::Parameter(string data, tokenType t)
{
	info = data;
	type = t;
}

Parameter::~Parameter() {

}

string Parameter::toString()//stringstream & out)
{
	ostringstream sb;
	if(type == STRING)
	{
		sb << "\'" << info << "\'";
	}
	else
	{
		sb << info;
	}
	return sb.str();
}


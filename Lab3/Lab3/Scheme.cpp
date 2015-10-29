

#include "Scheme.h"

using namespace std;

Scheme::Scheme() {
	// TODO Auto-generated constructor stub

}

Scheme::~Scheme() {
	// TODO Auto-generated destructor stub
}

void Scheme::setAttributes(vector<string>& s)
{
	myAttributes = s;
}

vector<string>& Scheme::getMyAttributes()
{
	return myAttributes;
}

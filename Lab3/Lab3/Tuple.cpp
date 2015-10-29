
#include "Tuple.h"

using namespace std;

Tuple::Tuple() {

}

Tuple::~Tuple() {

}

void Tuple::printTuple(stringstream& out, vector<string>& myAttributes, vector<size_t>& varIndex)
{
	if(varIndex.size() > 0) //there are variables to print
	{
		if(varIndex.size() < 2) //just 1
		{
			for(size_t i = 0; i < size(); i++)
			{
				out << "  " << myAttributes.at(varIndex.at(i)) << "='" << at(i) << "'" << endl;
			}
		}
		else
		{
			for(size_t i = 0; i < size(); i++)
			{
				if(i < size()-1) //if i we're not at the last index
				{
					if(i == 0) //first iteration
					{
						out << "  " << myAttributes.at(varIndex.at(i)) << "='" << at(i) << "', ";
					}
					else
					{
						out << myAttributes.at(varIndex.at(i)) << "='" << at(i) << "', ";
					}
				}
				else //we're at the last index
				{
					if(i == 0) //first time thru
					{
						out << "  " << myAttributes.at(varIndex.at(i)) << "='" << at(i) << "'" << endl;
					}
					else
					{
						out << myAttributes.at(varIndex.at(i)) << "='" << at(i) << "'" << endl;
					}
				}
			}
		}
	}
}

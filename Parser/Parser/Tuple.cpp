#include "Tuple.h"

using namespace std;

void Tuple::printTuple(stringstream & out, vector<string>& myAttributes, vector<size_t>& varIndex)
{
	if (varIndex.size() > 0) //there are variables to print
	{
		if (varIndex.size() < 2)
		{
			for (size_t i = 0; i < size(); i++)
			{
				out << "  " << myAttributes.at(varIndex.at(i)) << "='" << at(i) << "'" << endl;
			}
		}
		else
		{
			for (size_t i = 0; i < size(); i++)
			{
				if (i < size() - 1)
				{
					if (i == 0)
					{
						out << "  " << myAttributes.at(varIndex.at(i)) << "='" << at(i) << "', ";
					}
					else
					{
						out << myAttributes.at(varIndex.at(i)) << "='" << at(i) << "', ";
					}
				}
				else
				{
					if (i == 0)
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

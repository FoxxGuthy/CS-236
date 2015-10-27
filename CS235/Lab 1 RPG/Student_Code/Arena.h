#pragma once
#include "ArenaInterface.h"
#include "Archer.h"
#include "Robot.h"
#include "Cleric.h"
#include <sstream>

class Arena :
	public ArenaInterface
{
public:
	Arena();
	~Arena();

	bool addFighter(string info);
	bool removeFighter(string name);
	FighterInterface* getFighter(string name);
	int getSize()
	{
		return ArenaCollection.size();
	}


private:
	vector <FighterInterface*> ArenaCollection;

	
};


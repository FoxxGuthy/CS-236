#include "Arena.h"


Arena::Arena()
{
}


Arena::~Arena()
{
}

bool Arena::addFighter(string info)
{
	stringstream infoIntake(info);

	string newName, newType;
	int newMaxHP = 0, newStrength = 0, newSpeed = 0, newMagic = 0;

	infoIntake >> newName >> newType >> newMaxHP >> newStrength >> newSpeed >> newMagic;

	/**
	If intake of data false, there is nothing to compare or do, so function will return false and end. 
	we must also have positive values, aka, >1
	*/
	if (!infoIntake.eof())
		return false;

	if (infoIntake.fail())
		return false; 

	if (newMaxHP < 1 || newStrength < 1|| newSpeed < 1|| newMagic < 1)
		return false;
	
	/**
	Checks for duplicate names. if duplicate is found, returns false and function exits. 
	*/
	bool duplicateFighter = false;
	for (unsigned i = 0; i < ArenaCollection.size(); i++)
	{
		if (newName == ArenaCollection[i]->getName())
		{
			duplicateFighter = true;
			return false;
		}
	}

	/**
	If not duplicate found, and no data errors push back new Fighter.
	use type based switch statements and return true.
	I suppose this if statement is unnecessary because it should have exited by now. 
	*/
	if (!duplicateFighter)
	{
		/**
		Each type is defined by a single letter. 
		A - Archer
		R - Robot
		C- Cleric
		switch doesn't like strings, so its IF statements for us!
		*/

		if (newType == "A")
		{
			ArenaCollection.push_back(new Archer(newName, newType, newMaxHP, newStrength, newSpeed, newMagic));
			return true;
		}
			

		else if (newType == "R")
		{
			ArenaCollection.push_back(new Robot(newName, newType, newMaxHP, newStrength, newSpeed, newMagic));
			return true;
		}
			

		else if (newType == "C")
		{
			ArenaCollection.push_back(new Cleric(newName, newType, newMaxHP, newStrength, newSpeed, newMagic));
			return true;
		}
	}
	return false;
}

bool Arena::removeFighter(string name)
{
	for (unsigned i = 0; i < ArenaCollection.size(); i++)
	{
		if (name == ArenaCollection[i]->getName())
		{
			ArenaCollection.erase(ArenaCollection.begin() + i);
			return true;
		}
	}
	return false;
}

FighterInterface* Arena::getFighter(string name)
{
	for (unsigned i = 0; i < ArenaCollection.size(); i++)
	{
		if (name == ArenaCollection[i]->getName())
		{
			return ArenaCollection[i];
		}
	}
	return NULL;
}


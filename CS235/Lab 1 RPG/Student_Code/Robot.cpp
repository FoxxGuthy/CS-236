#include "Robot.h"


Robot::Robot(string newName, string newType, int newMaxHP, int newStrength, int newSpeed, int newMagic) : Fighter(newName, newType, newMaxHP, newStrength, newSpeed, newMagic)
{
	maxElectricity = magic * 2; // electricity defined to be magic * 2
	currentElectricity = maxElectricity;
	bonusDamage = 0;
}


Robot::~Robot()
{
}

bool Robot::useAbility()
{
	/**
	if electricity greater or equal to ability cost, bonus damage = strength * (current/max energy)^4. subtract energy cost after calculation.
	returns true if it was used. returns false if not used. 
	*/
	if (currentElectricity >= ROBOT_ABILITY_COST)
	{
		bonusDamage = (strength * pow((currentElectricity * (1.0) / maxElectricity), 4));
		currentElectricity -= ROBOT_ABILITY_COST;
		return true;
	}
	return false; 
}
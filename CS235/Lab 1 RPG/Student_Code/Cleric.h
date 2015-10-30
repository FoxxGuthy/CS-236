#pragma once
#include "Fighter.h"
class Cleric :
	public Fighter
{
public:
	Cleric(string newName, string newType, int newMaxHP, int newStrength, int newSpeed, int newMagic);
	~Cleric();

	int getDamage()
	{
		return magic;
	}
	void reset()
	{
		currentHP = maxHP;
		currentMana = maxMana;
	}

	void regenerate();
	bool useAbility();

protected:
	int maxMana;
	int currentMana;

};


#pragma once
#include "Fighter.h"
#include <math.h>
class Robot :
	public Fighter
{
public:
	Robot(string newName, string newType, int newMaxHP, int newStrength, int newSpeed, int newMagic);
	~Robot();

	int getDamage()
	{
		int damage = (strength + bonusDamage);
		bonusDamage = RESET_VALUE;
		return damage;
	}
	void reset()
	{
		currentElectricity = maxElectricity;
		bonusDamage = RESET_VALUE;
		currentHP = maxHP;
	}

	bool useAbility();

protected:
	int currentElectricity;
	int maxElectricity;
	int bonusDamage;
};


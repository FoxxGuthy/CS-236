#pragma once
#include "Fighter.h"
class Archer :
	public Fighter
{
public:
	Archer(string newName, string newType, int newMaxHP, int newStrength, int newSpeed, int newMagic);
	~Archer();

	int getDamage()
	{
		return (speed);
	}

	void reset()
	{
		currentHP = maxHP;
		speed = baseSpeed;
	}

	bool useAbility()
	{
		speed++;
		return true;
	}

protected:
	int baseSpeed;
};


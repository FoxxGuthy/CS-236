#include "Fighter.h"


Fighter::Fighter(string newName, string newType, int newMaxHP, int newStrength, int newSpeed, int newMagic)
{
	name = newName;
	type = newType;
	maxHP = newMaxHP;
	strength = newStrength;
	speed = newSpeed;
	magic = newMagic;
	currentHP = maxHP;
}


Fighter::~Fighter()
{
}

void Fighter::takeDamage(int damage)
{
	int const DODGE_FACTOR = 4;
	int const MIN_DAMAGE = 1;
	if (damage - (speed / DODGE_FACTOR) >= MIN_DAMAGE)
		currentHP -= (damage - (speed / DODGE_FACTOR));
	else
		currentHP -= MIN_DAMAGE;
}

void Fighter::regenerate()
{
	int const REGEN_FACTOR = 6;

	if ((strength / REGEN_FACTOR) >= MIN_INCREASE)
		currentHP += strength / REGEN_FACTOR;
	else
		currentHP += MIN_INCREASE;

	if (currentHP > maxHP)
		currentHP = maxHP;
}
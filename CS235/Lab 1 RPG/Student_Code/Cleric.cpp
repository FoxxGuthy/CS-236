#include "Cleric.h"


Cleric::Cleric(string newName, string newType, int newMaxHP, int newStrength, int newSpeed, int newMagic) : Fighter(newName, newType, newMaxHP, newStrength, newSpeed, newMagic)
{
	maxMana = magic * 5; // mana defined to be magic * 5
	currentMana = maxMana;
}


Cleric::~Cleric()
{
}

void Cleric::regenerate()
{
	/**
	Regnerate Health. Must increase by 1, but can't go over maxHP. HP goes up by strength / 6
	*/
	int const REGEN_FACTOR = 6;

	if ((strength / REGEN_FACTOR) >= MIN_INCREASE)
		currentHP += strength / REGEN_FACTOR;
	else
		currentHP += MIN_INCREASE;

	if (currentHP > maxHP)
		currentHP = maxHP;

	/**
	Regenerate Mana. Must increase by 1, but can't go over maxMana. Mana goes up by magic / 5
	*/

	int const MANA_REGEN_FACTOR = 5;

	if ((magic / MANA_REGEN_FACTOR) >= MIN_INCREASE)
		currentMana += (magic / MANA_REGEN_FACTOR);
	else
		currentMana += MIN_INCREASE;

	if (currentMana > maxMana)
		currentMana = maxMana;
}

bool Cleric::useAbility()
{
	/**
	we must have enough mana to use the ability
	if there is, currentHP is increased by magic / 3. or 1 if magic / 3 !>= 1. however if we're over the maxHP, we set it back to maxHP
	*/
	int const HP_REGEN_FACTOR = 3;
	if (currentMana >= CLERIC_ABILITY_COST)
	{
		currentMana -= CLERIC_ABILITY_COST;
		if ((magic / HP_REGEN_FACTOR) >= MIN_INCREASE)
			currentHP += (magic / HP_REGEN_FACTOR);
		else
			currentHP += MIN_INCREASE;

		if (currentHP > maxHP)
			currentHP = maxHP;

		return true;
	}

	/**
	There was not enough mana so the ability was not used. 
	*/
	else 
		return false;
}

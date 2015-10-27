#pragma once
#include "FighterInterface.h"

int const MIN_INCREASE = 1;
int const RESET_VALUE = 0;

class Fighter :
	public FighterInterface
{
public:
	Fighter(string newName, string newType, int newMaxHP, int newStrength, int newSpeed, int newMagic);
	~Fighter();

	/** 
	Getters of class. No calculation, so defined in .h
	*/
	string getName()
	{
		return name;
	}
	int getMaximumHP()
	{
		return maxHP;
	}
	int getCurrentHP()
	{
		return currentHP;
	}
	int getStrength()
	{
		return strength;
	}
	int getSpeed()
	{
		return speed;
	}
	int getMagic()
	{
		return magic;
	}

	/** 
	takeDamage function, unique, but requires calculations. defined in .cpp
		regnerate function. common to robot and archer. Cleric is unique. defined in .cpp due to calculation. 
	*/
	void takeDamage(int damage);
	virtual void regenerate();

	/**
	The following are all completely unique, and so will be pure virutal functions to be defined the child class
	*/
	virtual int getDamage() = 0;
	virtual void reset() = 0;
	virtual bool useAbility() = 0;

protected:
	string name;
	string type;
	int maxHP;
	int strength;
	int speed;
	int magic;
	int currentHP;
};


#include "Archer.h"


Archer::Archer(string newName, string newType, int newMaxHP, int newStrength, int newSpeed, int newMagic) : Fighter(newName, newType, newMaxHP, newStrength, newSpeed, newMagic)
{
	baseSpeed = newSpeed;
}


Archer::~Archer()
{
}

#include "Station.h"

bool const TESTING25 = false;
int const FULL = 5;

Station::Station()
{
}


Station::~Station()
{
}



bool Station::addToStation(int car)
{
	if (TESTING25)
	{
		cout << "begin addToStation " << car << endl;
	}
	bool duplicateS = railStack.isDuplicate(car);
	bool duplicateQ = railQueue.isDuplicate(car);
	bool duplicateD = railQueue.isDuplicate(car);

	bool noDuplicates = false;

	if (!duplicateS && !duplicateQ && !duplicateD)
		noDuplicates = true;

	if (currentCar == NO_CURRENT_CAR && noDuplicates && car > NO_CURRENT_CAR)
	{
		currentCar = car;
		return true;
	}
	else
		return false;
}

int Station::showCurrentCar()
{
	return currentCar;
}

bool Station::removeFromStation()
{
	if (TESTING25)
	{
		cout << "begin remove from station " << currentCar << endl;
	}
	if (currentCar == NO_CURRENT_CAR)
		return false;

	else
	{
		currentCar = NO_CURRENT_CAR;
		return true;
	}

}

/**
PART 2 -------------------------------------------------------------------------------------------------------------------
*/

bool Station::addToStack()
{
	if (railStack.size() < FULL && currentCar != NO_CURRENT_CAR)
	{
		railStack.push(currentCar);
		currentCar = NO_CURRENT_CAR;
		return true;
	}

	return false;
}

bool Station::removeFromStack()
{
	if (currentCar == NO_CURRENT_CAR && railStack.size() > NO_CURRENT_CAR)
	{
		currentCar = railStack.top();
		railStack.pop();
		return true;
	}
	return false;
}

int Station::showTopOfStack()
{
	if (TESTING25) cout << "show top of stack " << endl;
	return railStack.top();
}

int Station::showSizeOfStack()
{
	return railStack.size();
}

/**
PART 3 --------------------------------------------------------------------------------------------------------------------------------------
*/

bool Station::addToQueue()
{
	if (railQueue.size() < FULL && currentCar != NO_CURRENT_CAR)
	{
		railQueue.push(currentCar);
		currentCar = NO_CURRENT_CAR;
		return true;
	}

	return false;
}

bool Station::removeFromQueue()
{
	if (currentCar == NO_CURRENT_CAR && railQueue.size() > NO_CURRENT_CAR)
	{
		currentCar = railQueue.front();
		railQueue.pop();
		return true;
	}
	return false;
}

int Station::showTopOfQueue()
{
if (TESTING25) cout << "show top of queue " << endl;
	return railQueue.front();
}

int Station::showSizeOfQueue()
{
	return railQueue.size();
}

/**
PART 4 --------------------------------------------------------------------------------------------------------------------------------------
*/

bool Station::addToDequeLeft()
{
	if (railDeque.size() < FULL && currentCar != NO_CURRENT_CAR)
	{
		railDeque.addLeft(currentCar);
		currentCar = NO_CURRENT_CAR;
		return true;
	}

	return false;
}

bool Station::addToDequeRight()
{
	if (railDeque.size() < FULL && currentCar != NO_CURRENT_CAR)
	{
		railDeque.addRight(currentCar);
		currentCar = NO_CURRENT_CAR;
		return true;
	}

	return false;
}

bool Station::removeFromDequeLeft()
{
	if (currentCar == NO_CURRENT_CAR && railDeque.size() > NO_CURRENT_CAR)
	{
		currentCar = railDeque.showLeft();
		railDeque.removeLeft();
		return true;
	}
	return false;
}

bool Station::removeFromDequeRight()
{
	if (currentCar == NO_CURRENT_CAR && railDeque.size() > NO_CURRENT_CAR)
	{
		currentCar = railDeque.showRight();
		railDeque.removeRight();
		return true;
	}
	return false;
}

int Station::showTopOfDequeLeft()
{
if (TESTING25) cout << "show left " << endl;
	return railDeque.showLeft();
}

int Station::showTopOfDequeRight()
{
if (TESTING25) cout << "show right " << endl;
	return railDeque.showRight();
}

int Station::showSizeOfDeque()
{
	return railDeque.size();
}

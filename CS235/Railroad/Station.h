#pragma once
#include "StationInterface.h"
#include "Stack.h"
#include "Queue.h"
#include "Deque.h"

int const NO_CURRENT_CAR = -1;

class Station :
	public StationInterface
{
public:
	Station();
~Station();

bool addToStation(int car);

int showCurrentCar();

bool removeFromStation();

/**
Part 2 -------------------------------------------------------------------------------------------------------------------------------------------
*/

bool addToStack();
bool removeFromStack();
int showTopOfStack();
int showSizeOfStack();

/**
Part 3 -------------------------------------------------------------------------------------------------------------------------------------------
*/

bool addToQueue();
bool removeFromQueue();
int showTopOfQueue();
int showSizeOfQueue();

/**
Part 4 -------------------------------------------------------------------------------------------------------------------------------------------
*/

bool addToDequeLeft();
bool addToDequeRight();
bool removeFromDequeLeft();
bool removeFromDequeRight();
int showTopOfDequeLeft();
int showTopOfDequeRight();
int showSizeOfDeque();



private:
	int currentCar = -1;
	Stack railStack;
	Queue railQueue;
	Deque railDeque;


};


#pragma once
#include "StationInterface.h"
#include "newLinkedList.h"

bool const TESTstack = false;
class Stack 
{
public:
	Stack() {}
	~Stack() {}

void push(int value)
{
	if (TESTstack)
	{
		cout << "begin push stack" << value << endl;
	}
	idStack.insertHead(value);
}

void pop()
{
	if (TESTstack)
	{
		cout << "begin pop stack" << endl;
	}
	idStack.remove(idStack.at(0));
}

int top()
{
	if (TESTstack)
	{
		cout << "begin top" << idStack.at(0) << endl;
	}
	return idStack.at(0);
}

int size()
{
	return idStack.size();
}

bool empty()
{
	if (TESTstack)
	{
		cout << "begin  empty"  << endl;
	}
	if (idStack.size() == 0)
		return true;

	else
		return false;
}

bool isDuplicate(int value)
{
	return idStack.isDuplicate(value);
}

private:
	newLinkedList<int> idStack;
};


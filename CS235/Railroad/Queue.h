#pragma once
#include "StationInterface.h"
#include "newLinkedList.h"

const bool TESTqueue = false;

class Queue 
{
public:
	Queue() {}
	~Queue() {}

	void push(int value)
	{
		if (TESTqueue)
		{
			cout << "begin queue push" << value << endl;
		}
		idQueue.insertHead(value);
	}

	void pop()
	{
		if (TESTqueue)
		{
			cout << "begin queue pop" << endl;
		}
		if (idQueue.size() > 0)
			idQueue.remove(idQueue.at(idQueue.size() - 1));
	}

	int front()
	{
		if (TESTqueue)
		{
			cout << "begin queue front" << endl;
		}
		if (idQueue.size() > 0)
			return idQueue.at(idQueue.size() - 1);

		else
			return -1;
	}

	int back()
	{
		if (TESTqueue)
		{
			cout << "begin queue back" << endl;
		}
		return idQueue.at(0);
	}

	int size()
	{
		return idQueue.size();
	}

	bool isDuplicate(int value)
	{
		return idQueue.isDuplicate(value);
	}

private:
	newLinkedList <int> idQueue;
};


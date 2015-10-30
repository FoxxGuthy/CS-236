#pragma once
#include "StationInterface.h"
#include "newLinkedList.h"

bool const TESTDECK = false;

class Deque 
{
public:
	Deque() {}
	~Deque() {}

	void addLeft(int value)
	{
		if (TESTDECK)
		{
			cout << "begin deck addleft" << endl;
		}
		idDeque.insertHead(value);
	}

	void addRight(int value)
	{
		if (TESTDECK)
		{
			cout << "begin deck addright" << endl;
		}
		idDeque.insertTail(value);
	}

	void removeLeft()
	{
		if (TESTDECK)
		{
			cout << "begin remove Left" << endl;
		}
		idDeque.remove(idDeque.at(0));
	}

	void removeRight()
	{
		if (TESTDECK)
		{
			cout << "begin deck remove Right" << endl;
		}
		if (idDeque.size() > 0)
			idDeque.remove(idDeque.at(idDeque.size() - 1));
	}

	int showLeft()
	{
			if (TESTDECK)
			{
				cout << "begin deck showL" << endl;
			}
		if (idDeque.size() > 0)
			return idDeque.at(0);

		else
			return -1;
	}

	int showRight() 
	{
		if (TESTDECK)
		{
			cout << "begin deck showright" << endl;
		}
		if (idDeque.size() > 0)
			return idDeque.at(idDeque.size() - 1);

		else
			return -1;
	}

	int size()
	{
		return idDeque.size();
	}



private:
	newLinkedList<int> idDeque;
};


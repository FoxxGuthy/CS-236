#pragma once
#include "QSInterface.h"
#include <sstream>
#include <algorithm>

int const EMPTY = 0;
int const INVALID = -1;
string const emptySTRING = "";

class QuickSort :
	public QSInterface
{
public:
	QuickSort();
~QuickSort();

void sortAll();
int medianOfThree(int left, int right);
int partition(int left, int right, int pivot);
string getArray();
int getSize();
void addToArray(int value);
bool createArray(int size);
void clear();

private:
	int* list;
        int capacity = EMPTY;
        int currentSize = EMPTY;
        void selection_sort_array (int the_array[], int size); // used by med 3 to sort mini array of begin, middle, end
		void sortAll(int left, int right); // used by sortAll() so we can call recursive with new bounds
};


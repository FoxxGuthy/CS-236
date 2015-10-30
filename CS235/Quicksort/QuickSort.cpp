#include "QuickSort.h"

bool const TEST = false;

QuickSort::QuickSort()
{
	currentSize = EMPTY;
	capacity = EMPTY;
}

QuickSort::~QuickSort()
{
	clear();
}

void QuickSort::sortAll()
{
	if (TEST)
		cout << "sort all " << endl;

	if (currentSize == EMPTY) // if empty, do nothin
		return;

	else
	{
		int indexLeft = 0;
		int indexRight = currentSize - 1;

		sortAll(indexLeft, indexRight);
	}
    //cout << getArray() << endl;
}

int QuickSort::medianOfThree(int left, int right)
{
	if (TEST)
		cout << "med of 3 " << endl;

	if (currentSize == EMPTY) // if empty, do nothin
		return INVALID;

	else if (left < EMPTY || right < EMPTY)// out of bounds ( less than zero, or larger than size) 
		return INVALID;

	else if (right > currentSize - 1 || left > currentSize - 1) // right has to be size - 1
		return INVALID;

	else if (!(left < right)) // left must be smaller than right
		return INVALID;

	else
	{
		int middle = 0;
		middle = (left + right) / 2;

		if (TEST)
			cout << "index: " << left << " " << right << " middle " << middle << endl;

		if (TEST)
			cout << " " << list[left] << " " << list[right] << "  " << list[middle] << endl;


		if (middle == left) // special case where left and right are next to each other e.g. 3 and 4
		{
			if (list[left] < list[right]) // already in order
				return middle;

			else
			{
				int temp = list[left];
				list[left] = list[right];
				list[right] = temp;
			}

		}

		else // rest of the time
		{
			if (list[left] < list[middle] && list[middle] < list[right]) // left < middle < right. already in order
				return middle;

			int smallList[3] = { list[left], list[middle], list[right] };

			selection_sort_array(smallList, 3);

			list[left] = smallList[0];
			list[middle] = smallList[1];
			list[right] = smallList[2];
			//cout << smallList[0] << " " << smallList[1] << " " << smallList[2] << endl;
		}

		return middle;
	}

}

int QuickSort::partition(int left, int right, int pivot)
{
	//if (TEST)
		//cout << "partition" << endl;

	if (currentSize == EMPTY) // if empty, do nothin
		return INVALID;

	else if (left < EMPTY || right < EMPTY)// out of bounds ( less than zero, or larger than size)
		return INVALID;

	else if (right > currentSize - 1 || left > currentSize - 1) // right has to be size - 1
		return INVALID;

	else if (!(left < right)) // left must be smaller than right
		return INVALID;

	else if (pivot < left || pivot > right) // pivot must be inbetween left and right
		return INVALID;

	else if ((right - left) == 2) // if right is only 2 more than left, there's 3 things in array left, right pivot
	{
		return medianOfThree(left, right);
	}

	else
	{
		int i = left + 1;
		int j = right;

		do {
			while ((i != right) && !(list[left] < list[i]))
			{
				++i; // move i right as long as its not at the end, and until i(value) is greater than the pivot
			}
			while (list[left] < list[j])
			{
				--j; // move j left until we find something less than the pivot
			}
			if (i < j) // i is left of j
			{
				swap(list[i], list[j]); // swap i and j in list
			}
		} while (i < j); // iterate until i and j cross

		swap(list[left], list[j]); // swap j and the pivot

		return j;
	}
}

string QuickSort::getArray()
{
	if (TEST)
		cout << "get array" << endl;

	if (currentSize == EMPTY) // empty array
		return emptySTRING;

	ostringstream os;
	for (int i = 0; i < currentSize; i++)
	{
		os << list[i];
		if (i != currentSize - 1) // last itme in array, we don't want a comma
			os << ",";
	}

	return os.str();
}

int QuickSort::getSize()
{
	if (TEST)
		cout << "getsize" << endl;

	return currentSize;
}

void QuickSort::addToArray(int value)
{
	if (TEST)
		cout << "add to array " << value << endl;

	if (capacity != EMPTY) // can't add to a array that doesn't exist. could also probably say if lists == NULL
	{
		list[currentSize] = value; // potential issue. if size is already at capacity. 
		currentSize++;
	}
}

bool QuickSort::createArray(int size)
{
	if (TEST)
		cout << "create array" << endl;

	if (size < EMPTY) // must be positive index
		return false;

	// do we need to clear before creating new?

	else
	{
		if (TEST)
			cout << "clear forom create " << endl;

		//clear();
		list = new int[size];
		capacity = size;
		return true;
	}
}

void QuickSort::clear()
{
	if (TEST)
		cout << "clear " << endl;

	delete[] list;
	currentSize = EMPTY;
}

void QuickSort::selection_sort_array(int the_array[], int size)
{
	for (int fill = 0; fill != size - 1; ++fill)
	{
		int pos_min = fill;
		for (int next = fill + 1; next != size; ++next)
		{
			if (the_array[next] < the_array[pos_min])
			{
				pos_min = next;
			}
		}
		if (fill != pos_min)
			swap(the_array[pos_min], the_array[fill]);
	}
}

void QuickSort::sortAll(int left, int right)
{
	if (right - left > 1) // right index must be 1 more than left
	{
        //cout << "left: " << left << " right: " << right << endl;

        int pivot = partition(left, right, medianOfThree(left, right)); // put the middle in the correct spot
        //int pivot = medianOfThree(left, right);
		sortAll(left, pivot); //left subarray
		sortAll(pivot + 1, right); //right subarray
	}
}


// very small quick sort function
/**
void quickSort(int arr[], int left, int right) {
	  int i = left, j = right;
	  int tmp;
	  int pivot = arr[(left + right) / 2];

	  //partition
	  while (i <= j) {
			while (arr[i] < pivot)
				  i++;
			while (arr[j] > pivot)
				  j--;
			if (i <= j) {
				  tmp = arr[i];
				  arr[i] = arr[j];
				  arr[j] = tmp;
				  i++;
				  j--;
			}
	  };

	 // recursion
	  if (left < j)
			quickSort(arr, left, j);
	  if (i < right)
			quickSort(arr, i, right);
}
*/

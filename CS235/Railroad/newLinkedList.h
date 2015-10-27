/**
*
*
*
Ways to improve:

write a duplicate function, instead of recoding as often as I do
simply call the clear()function, instead of recopying
next time, use Testing = true to turn off my couts. 


*/



#pragma once
#include "LinkedListInterface.h"
#include <stdexcept>
#include <iostream>

using namespace std;

bool const TESTING = true;

template<class T>
class newLinkedList :
	public LinkedListInterface < T >
{
public:

	/**
	Because we are not using a .cpp file, we must use brackets on the constructors
	they are not just declared here, they are defined here
	*/
	newLinkedList<T>(){}
	~newLinkedList()
		{
		Node* temp = NULL;
		while (header != NULL && (header != tail))
		{
			temp = tail;
			tail->prev->next = NULL;
			tail = tail->prev;
			delete temp;
		}
		delete header;
		}

	void insertHead(T value)
	{
		//cout << "begin head, value: " << value << endl;
		/**
		If list is empty, aka if header isn't pointing to anything
		*/
		if (header == NULL)
		{
			header = new Node(value, NULL, NULL);
			tail = header;
		}

		/**
		If header is already pointing, we'll need to bump it back, and check for duplicates
		First statement searches for duplicates, the next will add the data if it is unique.
		*/
		else
		{
			bool duplicate = false;
			Node* traverse = header;
			Node* temp = NULL;

			do {
				if (traverse->data == value)
					duplicate = true;
				traverse = traverse->next;
			} while (traverse != NULL && duplicate == false);

			if (duplicate == false)
			{
				temp = header;
				header = new Node(value, NULL, NULL);
				header->next = temp;
				header->next->prev = header;
			}
			traverse = NULL;
			temp = NULL;
			delete traverse;
			delete temp;
		}	
		//cout << "end head" << endl << endl;
	}

	void insertTail(T value)
	{
		//cout << "		begin tail, value " << value << endl;
		/**
		Special case: List is empty, so we can just throw it right on.
		*/
		if (header == NULL)
		{
			header = new Node(value, NULL, NULL);
			tail = header;
		}

		else
		{
			/**
			Search again for duplicates. This need not be unique the way we traverse, we can reuse from
			insertHead
			*/
			bool duplicate = false;
			Node* traverse5 = header;
			do 	{
				if (traverse5->data == value)
					duplicate = true;
				traverse5 = traverse5->next;
			} while (traverse5 != NULL && duplicate == false);

			/**
			Traverse is already at the end of the list, so we don't have to repeat the process
			however, we need traverse to point to the last item, not point to the same thing as the last item (NULL)
			*/
			if (duplicate == false)
			{
				tail->next = new Node(value, NULL, tail);
				tail = tail->next;
			}
			traverse5 = NULL;
			delete traverse5;
		}
		//cout << "end tail" << endl;
	}

	void insertAfter(T value, T insertionNode)
	{
		//cout << "begin insert after value: " << value << "after value " << insertionNode << endl;
		/**
		Must check that value IS NOT in the list
		Must check that insertion Node IS in the list
		*/
		bool duplicate = false;
		Node* traverse = header;

		do {
			if (traverse->data == value)
				duplicate = true;
			traverse = traverse->next;
		} while (traverse != NULL && duplicate == false);

		traverse = NULL;
		delete traverse;
		/**
		Above checked that value was unique and IS NOT IN the list
		Below: checks that insertionNode IS IN the list, if found, inserts
		*/
		if (duplicate == false)
		{
			bool found = false;
			Node* traverse6 = header;
			Node* insertNode = NULL;
			while (traverse6 != NULL && found == false)
			{
				if (traverse6->data == insertionNode)
				{
					found = true;
					insertNode = traverse6;
				}
				traverse6 = traverse6->next;
			}
		
			traverse6 = NULL;
			delete traverse6;
			/**
			Will need 3 handlers. 
			One for just after header
			One for just after tail
			and then a general middle insertion. 

			the problem occurs that after tail it tries to assign the "next" which is just NULL and it seems to break. 
			*/
			if (found == true)
			{
				/**
				This will correctly handle insertion for items after head and middle. 
				However, will not insert after tail, because insert->next points @ NULL. NULL doesn't have a prev.
				*/
				Node *newNODE =NULL;
				newNODE = new Node(value, insertNode->next, insertNode);
				if (insertNode->next != NULL)
					insertNode->next->prev = newNODE;
				insertNode->next = newNODE;
				if (tail == insertNode)
					tail = newNODE;
				/*newNODE = NULL;
				delete newNODE;
				insertNODE = NULL;
				delete insertNODE;*/
			}
		}
	}

	void remove(T value) // experiencing error when last element is removed
	{
		//cout << "		begin remove " << value << endl;
		bool found = false;
		Node* traverse6 = header;
		Node* nodeRemoved = NULL;
		while (traverse6 != NULL && found == false)
		{
			if (traverse6->data == value)
			{
				found = true;
				nodeRemoved = traverse6;
			}
			traverse6 = traverse6->next;
		}

		if (found == true)
		{
			if (nodeRemoved->next == NULL) // if node removed is the last node, must reset tail
				tail = nodeRemoved->prev;

			if (nodeRemoved->prev == NULL) // if node removed was the first node, must reassign head
				header = nodeRemoved->next;

			if (nodeRemoved->prev != NULL) // if nodeRemoved is the first then node, removed->prev is NULL and has no next
				nodeRemoved->prev->next = nodeRemoved->next;

			if (nodeRemoved->next != NULL)
				nodeRemoved->next->prev = nodeRemoved->prev; // if nodeRemoved is the last node, then nodeRemoved->next is NULL, and has no prev

			
			delete nodeRemoved;
		}

		traverse6 = NULL;
		delete traverse6;
		//cout << "end remove" << endl;
	}

	void clear()
	{
		//cout << "begin clear" << endl;
		/*
		this will delete everything up to the header. we must delete the header seperate beacuse when tail == header, 
		header has no prev
		*/
		Node* temp = NULL;
		while (header != NULL && (header != tail))
		{
			temp = tail;
			tail->prev->next = NULL;
			tail = tail->prev;
			delete temp;
		}
		delete header;
		header = NULL;
		tail = NULL;
		//cout << "end clear" << endl;
	}

	T at(int index)
	{
		//cout << "begin at, index " << index << endl;
		if (header == NULL)
		{
			return -1;
			//cout << "exception thrown" << endl;
			//throw out_of_range("EMPTY");
		}

		Node* traverse2 = header;
		for (int i = 0; i < index; i++)
		{
			if (traverse2 == NULL)
				throw out_of_range("stupid");

			traverse2 = traverse2->next;
		}
		/**
		throw will exit the program if index > size or index < 0


		if (index < 0 || index >= size())
		throw std::out_of_range("stupid");
		*/
		if (traverse2 == NULL || index < 0 || index > size() - 1)
		{
			cout << "exception thrown" << endl;
			throw out_of_range("stupid");
		}
			
	//	cout << "end at, value: " << traverse2->data << endl;
		return traverse2->data;
	}

	int size()
	{
		//cout << "begin size" << endl;
		/***
		A simpler way to do this would be to have an overall variable, size. 
		would have been increased every time a node was added, or decreased everytime it was deleted
		*/
		Node* traverse4 = header;
		int sizeCount = 0;
		while (traverse4 != NULL)
		{
			traverse4 = traverse4->next;
			sizeCount++;
		} 
		traverse4 = NULL;
		delete traverse4;
		//cout << "end size: " << sizeCount << endl;
		return sizeCount;
	}

	bool isDuplicate(T value)
	{
		//cout << "begin duplicate" << endl;
		bool duplicate = false;
		Node* traverse = header;

		if (size() == 0)
			return false;
		
		do {
			if (traverse->data == value)
				duplicate = true;
			traverse = traverse->next;
		} while (traverse != NULL && duplicate == false);

		return duplicate;
	}

private:

	struct Node
	{
		T data;
		Node* next = NULL;
		Node* prev = NULL;

		Node(T data_item, Node* ptr_next, Node* ptr_prev)
		{
			data = data_item;
			next = ptr_next;
			prev = ptr_prev;
		}
	};

	Node* header = NULL;
	Node* tail = NULL;
};

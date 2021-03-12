#pragma once
#include"PriorityNode.h"
template<typename T>
class PriorityQueue

{
private:

	PriorityNode<T>* frontPtr;
	PriorityNode<T>*backPtr;
public:
	PriorityQueue();
	bool isEmpty() const;
	bool enqueue(const T& newEntry,double priority);
	bool dequeue(T& frntEntry);
	bool peekFront(T& frntEntry)  const;
	~PriorityQueue();

	//copy constructor
	PriorityQueue(const PriorityQueue<T> & LQ);

};
template<typename T>
PriorityQueue<T>::PriorityQueue()
{
	frontPtr=nullptr;
	backPtr=nullptr;
}
template<typename T>
bool PriorityQueue<T>::isEmpty() const
{
	return (frontPtr == nullptr);
}
template<typename T>
bool PriorityQueue<T>::enqueue(const T& newEntry,double priority)
{
	PriorityNode<T>* newNodePtr = new PriorityNode<T>(newEntry,priority);
	PriorityNode<T>*p;
	// Insert the new node
	if (isEmpty()||priority>frontPtr->getPriority())
	{
		newNodePtr->setNext(frontPtr);// The queue is empty
		frontPtr=newNodePtr;
	}
	else
		// The queue was not empty
		{
			p=frontPtr;
			while(p->getNext()&&p->getNext()->getPriority()>=priority)
			{
				p=p->getNext();
			}
			newNodePtr->setNext(p->getNext());
			p->setNext(newNodePtr);
			
		}
	return true;
} // end enqueue
template<typename T>
bool PriorityQueue<T>::dequeue(T& frntEntry)
{
	if (isEmpty())
		return false;

	PriorityNode<T>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();
	// Queue is not empty; remove front
	/*if (nodeToDeletePtr == backPtr)	 // Special case: last node in the queue
		backPtr = nullptr;
*/
	// Free memory reserved for the dequeued node
	delete nodeToDeletePtr;

	return true;

}
template<typename T>
bool PriorityQueue<T>::peekFront(T& frntEntry) const
{
	if (isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;

}
template<typename T>
PriorityQueue<T>::~PriorityQueue()
{
	T temp;

	//Free (Dequeue) all nodes in the queue
	while (dequeue(temp));
}
template<typename T>
PriorityQueue<T>::PriorityQueue(const PriorityQueue<T> & LQ)
{
	PriorityNode<T>* NodePtr = LQ.frontPtr;
	if (!NodePtr) //LQ is empty
	{
		frontPtr = backPtr = nullptr;
		return;
	}

	//insert the first node
	PriorityNode<T>* ptr = new PriorityNode<T>(NodePtr->getItem());
	frontPtr = backPtr = ptr;
	NodePtr = NodePtr->getNext();

	//insert remaining nodes
	while (NodePtr)
	{
		PriorityNode<T>* ptr = new PriorityNode<T>(NodePtr->getItem());
		backPtr->setNext(ptr);
		backPtr = ptr;
		NodePtr = NodePtr->getNext();
	}
}




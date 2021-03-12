#pragma once
#include"node.h"
template<typename T>
class PriorityNode: public Node<T>
{
	double Priority;
	PriorityNode<T>*next;
	
public:
	PriorityNode(const T & r_Item,double priority);
	void setPriority(double priority);
	double getPriority() const ;
	void setNext(PriorityNode<T>* nextNodePtr);
	PriorityNode<T>* getNext() const ;
	~PriorityNode();

};
template<typename T>
PriorityNode<T>::PriorityNode(const T & r_Item,double priority):Node(r_Item)
{
	Priority=priority;
}
template<typename T>
PriorityNode<T>::~PriorityNode()
{
}
template<typename T>
void PriorityNode<T>::setPriority(double priority)
{
	Priority=priority;
}
template <typename T>
double PriorityNode<T>::getPriority() const
{
	return Priority;
}
template<typename T>
PriorityNode<T>* PriorityNode<T>::getNext() const
{
	return next;
}
template < typename T>
void PriorityNode<T>::setNext(PriorityNode<T>* nextNodePtr)
{
	next = nextNodePtr;
} 
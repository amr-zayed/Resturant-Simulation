#include "Order.h"

Order::Order(int id, ORD_TYPE r_Type,int size):Size(size)
{
	ID = (id>0&&id<1000)?id:0;	//1<ID<999
	type = r_Type;
	status = WAIT;
	WatingTime = 0;
	ServTime = 0;
}

Order::~Order()
{
}

int Order::GetID()
{
	return ID;
}


ORD_TYPE Order::GetType() const
{
	return type;
}



void Order::setStatus(ORD_STATUS s)
{
	status = s;
}

ORD_STATUS Order::getStatus() const
{
	return status;
}

void Order::SetMoney(double mon)
{
	totalMoney=mon;
}

double Order::getMoney()
{
	return this->totalMoney;
}
int  Order::getArrivalTime()
{
	return this->ArrTime;
}
double Order::CalcPriority_VIP_order()
{
	return (totalMoney-double(ArrTime))/double(Size);
}
void Order::setArrivalTime(int time)
{
	ArrTime = time;
}

void Order::SetServTime(int time)
{
	ServTime=time;
}

int Order::GetServTime()
{
	return ServTime;
}
void Order::SetType(ORD_TYPE Otype)
{
	type = Otype;
}

int Order::GetSize() const
{
	return Size;
}

void Order::SetWaitingTime(int WT)
{
	WatingTime=WT;
}
int Order::getWaitingTime()
{
	return WatingTime;
}
int Order::getServingTime()
{
	return ServTime;
}
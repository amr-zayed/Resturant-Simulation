#include "Cook.h"


Cook::Cook(int orderbeforebreak,int breakduration):BreakDuration(breakduration),OrdersBeforeBreak(orderbeforebreak)
{
	ServedKamOrder=0;
	AvailabilityTime = 0;
	OrderAssignedTo=nullptr;
	CookStatus=SAFE;
}


Cook::~Cook()
{
}


int Cook::GetID() const
{
	return ID;
}


ORD_TYPE Cook::GetType() const
{
	return type;
}


void Cook::setID(int id)
{
	ID = id;
}

void Cook::setType(ORD_TYPE t)
{
	type = t;
}

void Cook::setSpeed(int s)
{
	speed=s;
}

int Cook::getAvailabilityTime()
{
	return AvailabilityTime;
}

void Cook::SetOrderAssignedTo(Order* ORD)
{
	OrderAssignedTo=ORD;
}

int Cook::GetSpeed()
{
	return speed;
}

void Cook::SetAvailabilityTime(int time)
{
	AvailabilityTime=time;
}

void Cook::SetCookStatus(COOK_STAT stat)
{
	CookStatus=stat;
}
COOK_STAT Cook::GetCookstatus()
{
	return CookStatus;
}
int Cook::GetBreakDuration()
{
	return BreakDuration;
}
int Cook::GetNo_SER_ORD()
{
	return ServedKamOrder;
}
int Cook::getOrdersBeforeBreak()
{
	return OrdersBeforeBreak;
}
Order* Cook::GetOrderAssignedTo()
{
	return OrderAssignedTo;
}

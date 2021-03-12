#include "ArrivalEvent.h"
#include "..\Rest\Restaurant.h"


ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType):Event(eTime, oID)
{
	OrdType = oType;
}

ArrivalEvent::ArrivalEvent(ORD_TYPE otyp,int atime,int id,int size,double money):Event(atime,id)
{
	OrdType=otyp;
	Size=size;
	OrdMoney=money;
}
void ArrivalEvent::Execute(Restaurant* pRest)
{
	Order* Ord = new Order(OrderID, OrdType, Size);
	Ord->SetMoney(OrdMoney);
	Ord->setArrivalTime(EventTime);
	if (OrdType==TYPE_VIP)
	{
		pRest->WaitingOrderVIPenqueue(Ord);
	}
	else if(OrdType == TYPE_NRM)
	{
		pRest->AddtoNormal_OrdersWaitingQueue(Ord);
	}
	else if (OrdType == TYPE_VGAN)
	{
		pRest->AddtoVegan_OrdersWaitingQueue(Ord);
	}
}

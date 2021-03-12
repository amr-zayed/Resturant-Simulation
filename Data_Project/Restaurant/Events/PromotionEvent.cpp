#include "PromotionEvent.h"
#include "..\Rest\Restaurant.h"

PromotionEvent::PromotionEvent(int TS,int id,double money): Event(TS,id)
{
	EXMoney=money;
}

void PromotionEvent::Execute(Restaurant* pRest)
{	
}

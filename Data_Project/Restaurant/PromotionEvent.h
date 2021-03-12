#pragma once

#include "Events\Event.h"
class PromotionEvent: public Event 
{
private:
	double EXMoney; //Extra money the customer willing to buy 
public:
	PromotionEvent(int TS,int id,double money);
	virtual void Execute(Restaurant *pRest);
};
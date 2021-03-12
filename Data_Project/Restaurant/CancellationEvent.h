#pragma once
#include "Events\Event.h"
class CancellationEvent :public Event
{
public:
	CancellationEvent(int ts,int id);
	virtual void Execute(Restaurant* prest);
};


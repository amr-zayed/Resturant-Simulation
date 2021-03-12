#ifndef __ORDER_H_
#define __ORDER_H_

#include "..\Defs.h"

class Order
{

protected:
	int ID;         //Each order has a unique ID (from 1 --> 999 )
	ORD_TYPE type;		//order type: Normal, vegan, VIP
	ORD_STATUS status;	//waiting, in-service, done
	double totalMoney;	//Total order money
	int ArrTime, ServTime, FinishTime;	//arrival, service start, and finish times

	///////////////////////////ADDED/////////////////////////
	int WatingTime;  //time spent in waiting 
	const int Size;   //Numer of dishes in the orders //To be COnst
public:
	Order(int ID, ORD_TYPE r_Type,int size);
	virtual ~Order();
	int GetID();
	ORD_TYPE GetType() const;
	void setStatus(ORD_STATUS s);
	ORD_STATUS getStatus() const;
	

	void SetMoney(double);
	//ADDED
	double getMoney();
	int getArrivalTime();
	int GetServTime();
	int GetSize() const;
	void SetType(ORD_TYPE);
	void SetWaitingTime(int);
	double CalcPriority_VIP_order();//Calculate the Priority of vip Order according to Equation
	void setArrivalTime(int);
	void SetServTime(int);
	int getWaitingTime();
	int getServingTime();
};

#endif
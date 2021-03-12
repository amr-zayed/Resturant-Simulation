#include <cstdlib>
#include <time.h>
#include<fstream>
#include <iostream>
using namespace std;

#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"
#include "../CancellationEvent.h"
#include"../Events/PromotionEvent.h"


Restaurant::Restaurant() 
{
	pGUI = NULL;
	O_waiting_count_VIP = 0;
	O_waiting_count_Normal = 0;
	O_waiting_count_Vegan = 0;
	C_Available_count_VIP = 0;
	C_Available_count_Normal = 0;
	C_Available_count_Vegan = 0;
	NormO_silent_counter = 0;
	VIPO_silent_counter = 0;
	VegO_silent_counter = 0;
	VIPC_silent_counter = 0;
	NormC_silent_counter = 0;
	VegC_silent_counter = 0;
	AutoPromoted_Count=0;
	Urgent_count=0;
	inj_C = 0;
	timestep=0;
	arrived_orders = 0;
	promoted_orders = 0;
}

void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE	mode = pGUI->getGUIMode();
		
	switch (mode)	//Add a function for each mode in next phases
	{
	case MODE_INTR:
		Interactive();
		pGUI->PrintMessage("SImulation ENDED. click to exit.");
		pGUI->waitForClick();
		break;
	case MODE_STEP:
		StepByStep();
		pGUI->PrintMessage("SImulation ENDED. click to exit.");
		pGUI->waitForClick();
		break;
	case MODE_SLNT:
		SilentMode();
		break;
	};

}



//////////////////////////////////  Event handling functions   /////////////////////////////

//Executes ALL events that should take place at current timestep
void Restaurant::ExecuteEvents(int CurrentTimeStep)
{
	Event *pE;
	while( EventsQueue.peekFront(pE) )	//as long as there are more events
	{
		if(pE->getEventTime() > CurrentTimeStep )	//no more events at current timestep
			return;

		pE->Execute(this);
		EventsQueue.dequeue(pE);	//remove event from the queue
		delete pE;		//deallocate event object from memory
	}

}


Restaurant::~Restaurant()
{
		if (pGUI)
			delete pGUI;
}

void Restaurant::FillDrawingList()
{
	//This function should be implemented in phase1
	//It should add ALL orders and Cooks to the drawing list
	//It should get orders from orders lists/queues/stacks/whatever (same for Cooks)
	//To add orders it should call function  void GUI::AddToDrawingList(Order* pOrd);
	//To add Cooks it should call function  void GUI::AddToDrawingList(Cook* pCc);

	//ADDING ORDERS TO DRAWING LIST
	int VIPID = -1; //variable to save in the ID of the first Order in the VIP queue updated every itiration
	int NRMID = -1; //variable to save in the ID of the first Order in the Normal queue updated every itiration
	int VGNID = -1; //variable to save in the ID of the first Order in the Vegan queue updated every itiration
	Order* temp=NULL;
	Order* frontVIP; //pointer to save in the first VIP Order in the queue and doesn't change to know that Ihave finish searching and got back to the first Cook
	Order* frontNRM; //pointer to save in the first Normal Order
	Order* frontVGN; //pointer to save in the first Vegan Order

	Order* frontVIPp; //Pointer to save in the first Order in the VIP queue and  changes every itiration 
	Order* frontNRMm; //Pointer to save in the first Order in the Normal queue and  changes every itiration 
	Order* frontVGNn; //Pointer to save in the first Order in the Vegan queue and  changes every itiration 

	VIP_OrdersWaiting.peekFront(frontVIPp);
	Normal_OrdersWaiting.peekFront(frontNRMm);
	Vegan_OrdersWaiting.peekFront(frontVGNn);
	VIP_OrdersWaiting.peekFront(frontVIP);
	Normal_OrdersWaiting.peekFront(frontNRM);
	Vegan_OrdersWaiting.peekFront(frontVGN);
	int frontVIPID;
	if (!VIP_OrdersWaiting.isEmpty())
	{
		frontVIPID = frontVIP->GetID();
	}
	else
	{
		frontVIPID = -1;
	}
	int frontNRMID;
	if (!Normal_OrdersWaiting.isEmpty())
	{
		frontNRMID = frontNRM->GetID();
	}
	else
	{
		frontNRMID = -1;
	}
	int frontVGNID;
	if (!Vegan_OrdersWaiting.isEmpty())
	{
		frontVGNID = frontVGN->GetID();
	}
	else
	 {
		frontVGNID = -1;
	}
	while (!(frontVIPID == VIPID && frontNRMID == NRMID && frontVGNID == VGNID))
	{
		if (frontVIPID!=VIPID && !VIP_OrdersWaiting.isEmpty())
		{
			VIP_OrdersWaiting.peekFront(temp);
		}
		if (frontNRMID!= NRMID && !Normal_OrdersWaiting.isEmpty())  ///The condition isEmpty is added for extra check only
		{
			if (temp && frontNRMm->getArrivalTime() < temp->getArrivalTime())
			{
				Normal_OrdersWaiting.peekFront(temp);
			}
			else if(temp && frontNRMm->getArrivalTime() == temp->getArrivalTime())
			{
				
			}
			else if(!temp)
			{
				Normal_OrdersWaiting.peekFront(temp);
			}
		}
		if (frontVGNID != VGNID && !Vegan_OrdersWaiting.isEmpty())
		{
			if (temp && frontVGNn->getArrivalTime()<temp->getArrivalTime())
			{
				Vegan_OrdersWaiting.peekFront(temp);
			}
			else if (temp && frontVGNn->getArrivalTime() == temp->getArrivalTime())
			{

			}
			else if(!temp)
			{
				Vegan_OrdersWaiting.peekFront(temp);
			}
		}
		if (temp&&temp->GetType() == TYPE_VIP)
		{
			VIP_OrdersWaiting.dequeue(temp);
			pGUI->AddToDrawingList(*&temp);
			VIP_OrdersWaiting.enqueue(temp);  //,temp->CalcPriority_VIP_order()
			VIP_OrdersWaiting.peekFront(frontVIPp);
			VIPID = frontVIPp->GetID();
		}
		else if(temp&&temp->GetType()==TYPE_NRM)
		{
			Normal_OrdersWaiting.dequeue(temp);
			pGUI->AddToDrawingList(*&temp);
			Normal_OrdersWaiting.enqueue(temp);
			Normal_OrdersWaiting.peekFront(frontNRMm);
			NRMID = frontNRMm->GetID();
		}
		else if (temp && temp->GetType() == TYPE_VGAN)
		{
			Vegan_OrdersWaiting.dequeue(temp);
			pGUI->AddToDrawingList(*&temp);
			Vegan_OrdersWaiting.enqueue(temp);
			Vegan_OrdersWaiting.peekFront(frontVGNn);
			VGNID = frontVGNn->GetID();
		}
		temp = NULL;
	}
	//ADDING COOKS TO DRAWING LIST////////////////////////////////////////////
	int VIPIDC = -1; //variable to save in the ID of the first Cook in the VIP queue updated every itiration
	int NRMIDC = -1; //variable to save in the ID of the first Cook in the Normal queue updated every itiration
	int VGNIDC = -1; //variable to save in the ID of the first Cook in the VIP queue updated every itiration
	Cook* tempC = NULL;
	Cook* frontVIPC; //pointer to save in the first VIP Cook in the queue and doesn't change to know that Ihave finish searching and got back to the first Cook
	Cook* frontNRMC; //pointer to save in first Normal Cook 
	Cook* frontVGNC; //pointer to save in the first Vegan Cook

	Cook* frontVIPpC; //Pointer to save in the first Cook in the VIP queue and  changes every itiration 
	Cook* frontNRMmC; //Pointer to save in the first Cook in the Normal queue and changes every itiration
	Cook* frontVGNnC; //Pointer to save in the first Cook in the Vegan queue and changes every itiration
	VIP_AvailableCook.peekFront(frontVIPpC);
	Normal_AvailableCook.peekFront(frontNRMmC);
	Vegan_AvailableCook.peekFront(frontVGNnC);


	VIP_AvailableCook.peekFront(frontVIPC);
	Normal_AvailableCook.peekFront(frontNRMC);
	Vegan_AvailableCook.peekFront(frontVGNC);
	int frontVIPIDC;
	if (!VIP_AvailableCook.isEmpty())
	{
		frontVIPIDC = frontVIPC->GetID();
	}
	else
	{
		frontVIPIDC = -1;
	}
	int frontNRMIDC;
	if (!Normal_AvailableCook.isEmpty())
	{
		frontNRMIDC = frontNRMC->GetID();
	}
	else
	{
		frontNRMIDC = -1;
	}
	int frontVGNIDC;
	if (!Vegan_AvailableCook.isEmpty())
	{
		frontVGNIDC = frontVGNC->GetID();
	}
	else
	{
		frontVGNIDC = -1;
	}
	while (!(frontVIPIDC == VIPIDC && frontNRMIDC == NRMIDC && frontVGNIDC == VGNIDC))
	{
		if (frontVIPIDC != VIPIDC && !VIP_AvailableCook.isEmpty())
		{
			VIP_AvailableCook.peekFront(tempC);
		}
		if (frontNRMIDC != NRMIDC && !Normal_AvailableCook.isEmpty())
		{
			if (tempC && frontNRMmC->getAvailabilityTime() < tempC->getAvailabilityTime())
			{
				Normal_AvailableCook.peekFront(tempC);
			}
			else if(tempC && frontNRMmC->getAvailabilityTime() == tempC->getAvailabilityTime())
			{

			}
			else if (!tempC)
			{
				Normal_AvailableCook.peekFront(tempC);
			}
		}
		if (frontVGNIDC != VGNIDC && !Vegan_AvailableCook.isEmpty())
		{
			if (tempC && frontVGNnC->getAvailabilityTime() < tempC->getAvailabilityTime())
			{
				Vegan_AvailableCook.peekFront(tempC);
			}
			else if (tempC && frontVGNnC->getAvailabilityTime() == tempC->getAvailabilityTime())
			{

			}
			else if (!tempC)
			{
				Vegan_AvailableCook.peekFront(tempC);
			}
			
		}
		if (tempC && tempC->GetType() == TYPE_VIP)
		{
			VIP_AvailableCook.dequeue(tempC);
			pGUI->AddToDrawingList(*&tempC);
			VIP_AvailableCook.enqueue(tempC);
			VIP_AvailableCook.peekFront(frontVIPpC);
			VIPIDC = frontVIPpC->GetID();
		}
		else if (tempC && tempC->GetType() == TYPE_NRM)
		{
			Normal_AvailableCook.dequeue(tempC);
			pGUI->AddToDrawingList(*&tempC);
			Normal_AvailableCook.enqueue(tempC);
			Normal_AvailableCook.peekFront(frontNRMmC);
			NRMIDC = frontNRMmC->GetID();
		}
		else if (tempC && tempC->GetType() == TYPE_VGAN)
		{
			Vegan_AvailableCook.dequeue(tempC);
			pGUI->AddToDrawingList(*&tempC);
			Vegan_AvailableCook.enqueue(tempC);
			Vegan_AvailableCook.peekFront(frontVGNnC);
			VGNIDC = frontVGNnC->GetID();
		}
		tempC = NULL;
	}
	//ADDING ORDERS IN SERVING TO DRAWING LIST
	int ID_Serving = -1; //variable to save the ID of the first order in each itiration
	Order* frontOrderServing; //pointer to save in the first Order of the original queue and doesnt change
	Order* temp_Serving; 
	Order* peekfirst; //pointer to save in the first Order in the queue each itiration 
	OrdersInServing.peekFront(frontOrderServing);

	int frontOrderServingID;
	if (!OrdersInServing.isEmpty())
	{
		frontOrderServingID = frontOrderServing->GetID();
	}
	else
	{
		frontOrderServingID = -1;
	}
	while (frontOrderServingID != ID_Serving)
	{
		OrdersInServing.dequeue(temp_Serving);
		pGUI->AddToDrawingList(*&temp_Serving);
		OrdersInServing.enqueue(temp_Serving);
		OrdersInServing.peekFront(peekfirst);
		ID_Serving = peekfirst->GetID();
	}
	peekfirst = NULL;
	temp_Serving = NULL;
	//ADDING THE FINISHED ORDERS TO THE DRAWING LIST
	int ID_Done = -1;
	Order* peekFirstDone;
	Order* temp_done;
	Order* firstOrderDone;
	int firstOrderDoneID;
	OrdersFinished.peekFront(firstOrderDone);
	if (!OrdersFinished.isEmpty())
	{
		firstOrderDoneID = firstOrderDone->GetID();
	}
	else
	{
		firstOrderDoneID = -1;
	}
	while (firstOrderDoneID != ID_Done)
	{
		OrdersFinished.dequeue(temp_done);
		pGUI->AddToDrawingList(*&temp_done);
		OrdersFinished.enqueue(temp_done);
		OrdersFinished.peekFront(peekFirstDone);
		ID_Done = peekFirstDone->GetID();
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	pGUI->UpdateInterface();
	pGUI->ResetDrawingList(); 
}


void Restaurant::SimpleSimulator()
{
	LoadingFunction();
	Event* current_event;
	Order* gettingserviced;
	bool FIRST_NORMAL=true;
	bool FIRST_VIP=true;
	bool FIRST_VEGAN=true;
	while (EventsQueue.peekFront(current_event) != false || OrdersInServing.isEmpty() == false || VIP_OrdersWaitingPriorityQueue.isEmpty() == false || Normal_OrdersWaiting.isEmpty() == false || Vegan_OrdersWaiting.isEmpty() == false)// Not equal elmafrood ya amr #zaki
	{
		if (timestep==0)
		{pGUI->PrintMessage("Current Time Step : " + to_string(timestep), "Number of available VIP orders: " + to_string(O_waiting_count_VIP) + ",  Number of waiting Normal orders: " + to_string(O_waiting_count_Normal) + ",  Number of waiting Vegan orders: " + to_string(O_waiting_count_Vegan), "Number of available VIP cooks: 0,  Number of available Normal cooks: 0,  Number of available Vegan cooks: 0");}
		else
		{pGUI->PrintMessage("Current Time Step : " + to_string(timestep), "Number of available VIP orders: " + to_string(O_waiting_count_VIP) + ",  Number of waiting Normal orders: " + to_string(O_waiting_count_Normal) + ",  Number of waiting Vegan orders: " + to_string(O_waiting_count_Vegan), "Number of available VIP cooks: " + to_string(C_Available_count_VIP) + ",  Number of available Normal cooks: " + to_string(C_Available_count_Normal) + ",  Number of available Vegan cooks: " + to_string(C_Available_count_Vegan));}
		if (EventsQueue.peekFront(current_event) != false && timestep == current_event->getEventTime())
		{
			EventsQueue.dequeue(current_event);
			//current_event->Execute(this);
			if (current_event == dynamic_cast<CancellationEvent*> (current_event))
			{
				current_event->Execute(this);
				canceled_orders++;
			}
			if (current_event == dynamic_cast<ArrivalEvent*> (current_event))
			{
				current_event->Execute(this);
				arrived_orders++;
			}
			if(current_event==dynamic_cast<PromotionEvent*>(current_event))
			{
				current_event->Execute(this);
			}
			
		}
		else /*if(timestep <= current_event->getEventTime())*/
		{
			pGUI->waitForClick();
	FIRST_NORMAL=true;
	FIRST_VIP=true;
	FIRST_VEGAN=true;
	assignmentfunction();
			timestep++;
		}
		//pGUI->PrintMessage(Timestep, "Number of available VIP orders: " + to_string(O_waiting_count_VIP) + ",  Number of waiting Normal orders: " + to_string(O_waiting_count_Normal) + ",  Number of waiting Vegan orders: " + to_string(O_waiting_count_Vegan), "Number of available VIP cooks: " + to_string(C_Available_count_VIP) + ",  Number of available Normal cooks: " + to_string(C_Available_count_Normal) + ",  Number of available Vegan cooks: " + to_string(C_Available_count_Vegan));
		FillDrawingList();
		//while(!VIP_OrdersWaiting.isEmpty()||!Normal_OrdersWaiting.isEmpty()||!Vegan_OrdersWaiting.isEmpty()||!OrdersInServing.isEmpty())
			//assignmentfunction();
		}

		//pGUI->PrintMessage(Timestep, "Number of available VIP orders: " + to_string(O_waiting_count_VIP) + ",  Number of waiting Normal orders: " + to_string(O_waiting_count_Normal) + ",  Number of waiting Vegan orders: " + to_string(O_waiting_count_Vegan), "Number of available VIP cooks: " + to_string(C_Available_count_VIP) + ",  Number of available Normal cooks: " + to_string(C_Available_count_Normal) + ",  Number of available Vegan cooks: " + to_string(C_Available_count_Vegan));
		FillDrawingList();
}



//////////////////////////////////////////////////////////////////////////////////////////////
/// ==> 
///  DEMO-related functions. Should be removed in phases 1&2

//Begin of DEMO-related functions

//This is just a demo function for project introductory phase
//It should be removed starting phase 1
/*void Restaurant::Just_A_Demo()
{
	
	//
	// THIS IS JUST A DEMO FUNCTION
	// IT SHOULD BE REMOVED IN PHASE 1 AND PHASE 2
	
	int EventCnt;	
	Order* pOrd;
	Event* pEv;
	srand(time(NULL));

	pGUI->PrintMessage("Just a Demo. Enter EVENTS Count(next phases should read I/P filename):");
	EventCnt = atoi(pGUI->GetString().c_str());	//get user input as a string then convert to integer

	pGUI->PrintMessage("Generating Events randomly... In next phases, Events should be loaded from a file...CLICK to continue");
	pGUI->waitForClick();
		
	//Just for sake of demo, generate some cooks and add them to the drawing list
	//In next phases, Cooks info should be loaded from input file
	int C_count = 12;	
	Cook **pC=new Cook*[C_count];
	int cID = 1;

	for(int i=0; i<C_count; i++)
	{
		cID+= (rand()%15+1);
		pC[i]=new Cook(5,3);
		pC[i]->setID(cID);
		pC[i]->setType((ORD_TYPE)(rand()%TYPE_CNT));
	}	
	
	
		
	int EvTime = 0;

	int O_id = 1;
	
	//Create Random events and fill them into EventsQueue
	//All generated event will be "ArrivalEvents" for the demo
	for(int i=0; i<EventCnt; i++)
	{
		O_id += (rand()%4+1);		
		int OType = rand()%TYPE_CNT;	//Randomize order type		
		EvTime += (rand()%5+1);			//Randomize event time
		pEv = new ArrivalEvent(EvTime,O_id,(ORD_TYPE)OType);
		EventsQueue.enqueue(pEv);

	}	

	// --->   In next phases, no random generation is done
	// --->       EventsQueue should be filled from actual events loaded from input file

	
	
	
	
	//Now We have filled EventsQueue (randomly)
	int CurrentTimeStep = 1;
	

	//as long as events queue is not empty yet
	while(!EventsQueue.isEmpty())
	{
		//print current timestep
		char timestep[10];
		itoa(CurrentTimeStep,timestep,10);	
		pGUI->PrintMessage(timestep);


		//The next line may add new orders to the DEMO_Queue
		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step
		

/////////////////////////////////////////////////////////////////////////////////////////
		/// The next code section should be done through function "FillDrawingList()" once you
		/// decide the appropriate list type for Orders and Cooks
		
		//Let's add ALL randomly generated Cooks to GUI::DrawingList
		for(int i=0; i<C_count; i++)
			pGUI->AddToDrawingList(*&pC[i]);
		
		//Let's add ALL randomly generated Ordes to GUI::DrawingList
		int size = 0;
		Order** Demo_Orders_Array = DEMO_Queue.toArray(size);
		
		for(int i=0; i<size; i++)
		{
			pOrd = Demo_Orders_Array[i];
			pGUI->AddToDrawingList(pOrd);
		}
/////////////////////////////////////////////////////////////////////////////////////////

		pGUI->UpdateInterface();
		Sleep(1000);
		CurrentTimeStep++;	//advance timestep
		pGUI->ResetDrawingList();
	}

	delete []pC;


	pGUI->PrintMessage("generation done, click to END program");
	pGUI->waitForClick();

	
}*/
////////////////

/*void Restaurant::AddtoDemoQueue(Order *pOrd)
{
	DEMO_Queue.enqueue(pOrd);
}*/

/// ==> end of DEMO-related function
//////////////////////////////////////////////////////////////////////////////////////////////

//ADDED MEMBER FUNCTIONS TO ENQUEUE IN THE NEWELLY CREATED QUEUES
void Restaurant::AddtoNormal_OrdersWaitingQueue(Order* ord)
{
	Normal_OrdersWaiting.enqueue(ord);
	O_waiting_count_Normal++;
	NormO_silent_counter++;
}
void Restaurant::AddtoVegan_OrdersWaitingQueue(Order* ord)
{
	Vegan_OrdersWaiting.enqueue(ord);
	O_waiting_count_Vegan++;
	VegO_silent_counter++;
}


void Restaurant::promotion(int id, double moneyExtra)
{
	Order* OrderToPromote;
	Order* firstOrder;
	int firstOrderID;
	Normal_OrdersWaiting.peekFront(firstOrder);
		if (!Normal_OrdersWaiting.isEmpty())
		{
			firstOrderID = firstOrder->GetID();
		}
		else
		{
			firstOrderID = -1;
		}
		Order* checkPeek = new Order(0, TYPE_NRM, 0);
		if (firstOrderID == id)
		{
			Normal_OrdersWaiting.dequeue(OrderToPromote);
			OrderToPromote->SetType(TYPE_VIP);
			WaitingOrderVIPenqueue(OrderToPromote);
			//VIP_OrdersWaiting.enqueue(OrderToPromote);  //,OrderToPromote->CalcPriority_VIP_order()
			OrderToPromote->SetMoney(OrderToPromote->getMoney() + moneyExtra);
			Normal_OrdersWaiting.peekFront(firstOrder);
			firstOrderID = firstOrder->GetID();
		}
		while (checkPeek->GetID() != firstOrderID && Normal_OrdersWaiting.dequeue(OrderToPromote))
		{
			if (OrderToPromote->GetID() == id)
			{
				OrderToPromote->SetType(TYPE_VIP);
				WaitingOrderVIPenqueue(OrderToPromote);
				//VIP_OrdersWaiting.enqueue(OrderToPromote);  //,OrderToPromote->CalcPriority_VIP_order()
				OrderToPromote->SetMoney(OrderToPromote->getMoney() + moneyExtra);
			}
			else
			{
				Normal_OrdersWaiting.enqueue(OrderToPromote);

			}
			Normal_OrdersWaiting.peekFront(checkPeek);
		}
	
}

void Restaurant::LoadingFunction()
{
	ifstream file ("TestCase_6.txt");
	int NumOfCooksVIP,NumOfCooksNormal,NumOfCooksVegan;
	int SpeedNormalMin,SpeedNormalMax,SpeedVeganMin,SpeedVeganMax,SpeedVIPMin,SpeedVIPMax;
	int OrderBeforeBreak;
	int BreakNormalMin,BreakNormalMax,BreakveganMin,BreakveganMax,BreakVIPMin,BreakVIPMax;
	double InjuryProb;
	int RestPeriodInjured;
	int TimeBeforeProm,TimeBeforePromVIP;
	int NumOfEvents;


	file>>NumOfCooksNormal>>NumOfCooksVegan>>NumOfCooksVIP;
	file>>SpeedNormalMin>>SpeedNormalMax>>SpeedVeganMin>>SpeedVeganMax>>SpeedVIPMin>>SpeedVIPMax;
	file>>OrderBeforeBreak;
	file>>BreakNormalMin>>BreakNormalMax>>BreakveganMin>>BreakveganMax>>BreakVIPMin>>BreakVIPMax;
	file>>InjuryProb>>RestPeriodInjured;
	file>>TimeBeforeProm>>TimeBeforePromVIP;
	file>>NumOfEvents;



	Cook* newcook;
	
	for(int i=0;i<NumOfCooksVIP;i++)
	{
		newcook=new Cook(OrderBeforeBreak, (rand()%(BreakVIPMax-BreakVIPMin + 1) + BreakVIPMin) );
		newcook->setSpeed(rand()%(SpeedVIPMax-SpeedVIPMin + 1) + SpeedVIPMin);
		newcook->setType(TYPE_VIP);
		newcook->setID(i+1);
		VIP_AvailableCook.enqueue(newcook);
	}
	for(int i=0;i<NumOfCooksNormal;i++)
	{
		newcook=new Cook(OrderBeforeBreak, (rand()%(BreakNormalMax-BreakNormalMin + 1) + BreakNormalMin) );
		newcook->setSpeed(rand()%(SpeedNormalMax-SpeedNormalMin + 1) + SpeedNormalMin);
		newcook->setType(TYPE_NRM);
		newcook->setID(NumOfCooksVIP+i+1);
		Normal_AvailableCook.enqueue(newcook);
	}
	for(int i=0;i<NumOfCooksVegan;i++)
	{
		newcook=new Cook(OrderBeforeBreak, (rand()%(BreakveganMax-BreakveganMin + 1) + BreakveganMin) );
		newcook->setSpeed(rand()%(SpeedVeganMax-SpeedVeganMin + 1) + SpeedVeganMin);
		newcook->setType(TYPE_VGAN);
		newcook->setID(NumOfCooksNormal+NumOfCooksVIP+i+1);
		Vegan_AvailableCook.enqueue(newcook);
	}
	InjuryProbability=InjuryProb;
	CookRestPeriodInjury=RestPeriodInjured;
	VIP_MaxWaitingTime=TimeBeforePromVIP;
	TimeBeforePromotion=TimeBeforeProm;
	C_Available_count_VIP = NumOfCooksVIP;
	C_Available_count_Normal = NumOfCooksNormal;
	C_Available_count_Vegan = NumOfCooksVegan;
	VIPC_silent_counter = NumOfCooksVIP;
	NormC_silent_counter = NumOfCooksNormal;
	VegC_silent_counter = NumOfCooksVegan;
	int eventscounter=0;
	char WhichEvent,OrderType;
	int Executiontime,id;
	int OrdSiz;
	double Money;
	while(eventscounter!=NumOfEvents)
	{
		file >>WhichEvent;
		Event* addevent;
		switch(WhichEvent)
		{
			case ('R'):
				file>>OrderType>>Executiontime>>id>>OrdSiz>>Money;
				if (OrderType=='N')
				{
					addevent=new ArrivalEvent(TYPE_NRM,Executiontime,id,OrdSiz,Money);
				}
				else if (OrderType=='G')
				{
					addevent=new ArrivalEvent(TYPE_VGAN,Executiontime,id,OrdSiz,Money);
				}
				else if (OrderType=='V')
				{
					addevent=new ArrivalEvent(TYPE_VIP,Executiontime,id,OrdSiz,Money);
				}
				EventsQueue.enqueue(addevent);
				break;
			case('X'):
				file>>Executiontime>>id;
				addevent=new CancellationEvent(Executiontime,id);
				EventsQueue.enqueue(addevent);
				break;
			case('P'):
				file>>Executiontime>>id>>Money;
				addevent=new PromotionEvent(Executiontime,id,Money);
				EventsQueue.enqueue(addevent);
				break;
		}
		eventscounter++;
	}
}
void Restaurant::OutputFunction()
{
	float AvgWait;
	float AvgServ;
	float AutoPromotedPercent;
	Order* doneOrders;
	float waitingTim=0;
	float servingTim=0;
	//////////////////////////////////////////////////////////////////
	int ID_Done = -1;
	Order* peekFirstDone;
	Order* temp_done;
	Order* firstOrderDone;
	int firstOrderDoneID;
	int numF=0;
	OrdersFinished.peekFront(firstOrderDone);
	if (!OrdersFinished.isEmpty())
	{
		firstOrderDoneID = firstOrderDone->GetID();
	}
	else
	{
		firstOrderDoneID = -1;
	}
	while (firstOrderDoneID != ID_Done)
	{
		OrdersFinished.dequeue(temp_done);
		waitingTim+=temp_done->getWaitingTime();
		servingTim+=temp_done->getServingTime();
		numF++;
		OrdersFinished.enqueue(temp_done);
		OrdersFinished.peekFront(peekFirstDone);
		ID_Done = peekFirstDone->GetID();
	}
	/////////////////////////////////////////////////////////////////
	AvgWait = (waitingTim/numF);
	AvgServ = (servingTim/numF);
	AutoPromotedPercent = (AutoPromoted_Count / numF) * 100;
	ofstream OutPutFile;
	OutPutFile.open("OutPutFile");
	OutPutFile << "FT"<<"	"<<"ID"<<"	"<<"AT"<<"	"<<"WT"<<"	"<<"ST"<<endl;
	PriorityDESQueue<Order*> doneToOutput;
	
	while (OrdersFinished.dequeue(doneOrders))
	{
		doneToOutput.enqueue(doneOrders, doneOrders->getWaitingTime() + doneOrders->GetServTime());
	}
	
	while (doneToOutput.dequeue(doneOrders))
	{
		OutPutFile << doneOrders->getWaitingTime() + doneOrders->GetServTime() << "	" << doneOrders->GetID() << "	" << doneOrders->getArrivalTime() << "	" << doneOrders->getWaitingTime() << "	" << doneOrders->getServingTime() << endl;
	}
	OutPutFile << "Orders: " << NormO_silent_counter+VIPO_silent_counter+VegO_silent_counter << "	" << "[ Norm: " << NormO_silent_counter << ", " << "Veg: " << VegO_silent_counter << ", " << "VIP: " << VIPO_silent_counter << " ]" << endl;
	OutPutFile << "cooks: " << (VIPC_silent_counter + NormC_silent_counter + VegC_silent_counter) << "	" << "[ Norm: " << NormC_silent_counter << ", " << "Veg: " << VegC_silent_counter << ", " << "VIP: " << VIPC_silent_counter <<", injured: "<< inj_C << " ]" << endl;
	OutPutFile << "Avg Wait= " << AvgWait << ", " << "Avg Serv= " << AvgServ << endl;
	OutPutFile << "Urgent Orders: " << Urgent_count << ", " << "Auto-promotion: " << AutoPromotedPercent << "%" << endl;
	OutPutFile.close();
}
void  Restaurant::SilentMode()
{
	int tracker=0;
	LoadingFunction();
	Event* current_event;
	Order* gettingserviced;
	bool FIRST_NORMAL = true;
	bool FIRST_VIP = true;
	bool FIRST_VEGAN = true;
	while (EventsQueue.peekFront(current_event) != false || OrdersInServing.isEmpty()==false || VIP_OrdersWaitingPriorityQueue.isEmpty()==false||Normal_OrdersWaiting.isEmpty()==false||Vegan_OrdersWaiting.isEmpty()==false)
	{
		
		if (EventsQueue.peekFront(current_event) != false && timestep == current_event->getEventTime())
		{
			EventsQueue.dequeue(current_event);
			if (current_event == dynamic_cast<CancellationEvent*> (current_event))
			{
				current_event->Execute(this);
				canceled_orders++;
			}
			if (current_event == dynamic_cast<ArrivalEvent*> (current_event))
			{
				current_event->Execute(this);
				arrived_orders++;
			}
			if (current_event == dynamic_cast<PromotionEvent*>(current_event))
			{
				current_event->Execute(this);
			}
		}
		else 
		{	
			assignmentfunction();
			timestep++;
		}
		tracker++;
	}
	OutputFunction();
}

void Restaurant::Interactive()
{
	LoadingFunction();
	Event* current_event;
	Order* gettingserviced;
	while (EventsQueue.peekFront(current_event) || OrdersInServing.peekFront(gettingserviced) || VIP_OrdersWaiting.peekFront(gettingserviced) || Normal_OrdersWaiting.peekFront(gettingserviced) || Vegan_OrdersWaiting.peekFront(gettingserviced))
	{
		pGUI->PrintMessage("Current Time Step : " + to_string(timestep), "Number of available VIP orders: " + to_string(O_waiting_count_VIP) + ",  Number of waiting Normal orders: " + to_string(O_waiting_count_Normal) + ",  Number of waiting Vegan orders: " + to_string(O_waiting_count_Vegan), "Number of available VIP cooks: " + to_string(C_Available_count_VIP) + ",  Number of available Normal cooks: " + to_string(C_Available_count_Normal) + ",  Number of available Vegan cooks: " + to_string(C_Available_count_Vegan));


		while (EventsQueue.peekFront(current_event) && current_event->getEventTime() == timestep)
		{
			/*if (EventsQueue.isEmpty())
				return;*/
			EventsQueue.dequeue(current_event);
			if (current_event == dynamic_cast<CancellationEvent*> (current_event))
			{
				current_event->Execute(this);
				canceled_orders++;
			}
			if (current_event == dynamic_cast<ArrivalEvent*> (current_event))
			{
				current_event->Execute(this);
				arrived_orders++;
			}
			if (current_event == dynamic_cast<PromotionEvent*>(current_event))
			{
				current_event->Execute(this);
				promoted_orders++;
			}
		}

		{
			assignmentfunction();
			FillDrawingList();
			pGUI->waitForClick();
			timestep++;

		}

	}
	OutputFunction();
}


void Restaurant::StepByStep()
{
	LoadingFunction();
	Event* current_event;
	Order* gettingserviced;
	while (EventsQueue.peekFront(current_event) || OrdersInServing.peekFront(gettingserviced) || VIP_OrdersWaiting.peekFront(gettingserviced) || Normal_OrdersWaiting.peekFront(gettingserviced) || Vegan_OrdersWaiting.peekFront(gettingserviced))
	{
		pGUI->PrintMessage("Current Time Step : " + to_string(timestep), "Number of available VIP orders: " + to_string(O_waiting_count_VIP) + ",  Number of waiting Normal orders: " + to_string(O_waiting_count_Normal) + ",  Number of waiting Vegan orders: " + to_string(O_waiting_count_Vegan), "Number of available VIP cooks: " + to_string(C_Available_count_VIP) + ",  Number of available Normal cooks: " + to_string(C_Available_count_Normal) + ",  Number of available Vegan cooks: " + to_string(C_Available_count_Vegan));
		while (EventsQueue.peekFront(current_event) && current_event->getEventTime() == timestep)
		{
			/*if (EventsQueue.isEmpty())
				return;*/
			EventsQueue.dequeue(current_event);
			if (current_event == dynamic_cast<CancellationEvent*> (current_event))
			{
				current_event->Execute(this);
				canceled_orders++;
			}
			if (current_event == dynamic_cast<ArrivalEvent*> (current_event))
			{
				current_event->Execute(this);
				arrived_orders++;
			}
			if (current_event == dynamic_cast<PromotionEvent*>(current_event))
			{
				current_event->Execute(this);
				promoted_orders++;
			}
		}

		{
			assignmentfunction();
			Sleep(1000);
			timestep++;
			FillDrawingList();


		}

	}
	OutputFunction();
}
void Restaurant::CancelOrder(int id)
{
	Order* deleteorder;
	Order* normalord;
	if (!Normal_OrdersWaiting.dequeue(normalord))
	{
		return;
	}
	if (normalord->GetID()==id)
	{
		delete normalord;
		return;
	}
	Normal_OrdersWaiting.enqueue(normalord);
	Normal_OrdersWaiting.peekFront(deleteorder);
	while(deleteorder->GetID()!=id && deleteorder!=normalord)
	{
		Normal_OrdersWaiting.dequeue(deleteorder);
		Normal_OrdersWaiting.enqueue(deleteorder);
		Normal_OrdersWaiting.peekFront(deleteorder);
	}
	if (deleteorder->GetID()==normalord->GetID())
	{
		return;
	}
	delete deleteorder;
	Normal_OrdersWaiting.peekFront(deleteorder);
	while (deleteorder!=normalord)
	{
		Normal_OrdersWaiting.dequeue(deleteorder);
		Normal_OrdersWaiting.enqueue(deleteorder);
		Normal_OrdersWaiting.peekFront(deleteorder);
	}
}
Order* Restaurant::WaitingOrderVIPdequeue()
{
	Order* neworder;
	Order* check;
	Queue <Order*> tempqueue;
	VIP_OrdersWaiting.dequeue(neworder);
	VIP_OrdersWaitingPriorityQueue.peekFront(check);

	while(neworder!=check)
	{
		VIP_OrdersWaitingPriorityQueue.dequeue(check);
		tempqueue.enqueue(check);
		VIP_OrdersWaitingPriorityQueue.peekFront(check);
	}
	VIP_OrdersWaitingPriorityQueue.dequeue(check);
	while (tempqueue.dequeue(check))
	{
		VIP_OrdersWaitingPriorityQueue.enqueue(check,check->CalcPriority_VIP_order());
	}
	return neworder;
}

Order* Restaurant::WaitingOrderVIPdequeuePriority()
{
	Order* neworder;
	Order* check=nullptr;
	Order* first;
	VIP_OrdersWaitingPriorityQueue.dequeue(neworder);
	VIP_OrdersWaiting.peekFront(first);
	if (first==neworder)
	{
		VIP_OrdersWaiting.dequeue(neworder);
		return neworder;
	}
	while(first!=check)
	{
		VIP_OrdersWaiting.dequeue(check);
		VIP_OrdersWaiting.enqueue(check);
		VIP_OrdersWaiting.peekFront(check);
		if (check==neworder)
		{
			VIP_OrdersWaiting.dequeue(neworder);
			VIP_OrdersWaiting.peekFront(check);
		}
	}
	return neworder;
}

void Restaurant::WaitingOrderVIPenqueue(Order* addorder)
{
	VIP_OrdersWaiting.enqueue(addorder);
	VIP_OrdersWaitingPriorityQueue.enqueue(addorder,addorder->CalcPriority_VIP_order());
	O_waiting_count_VIP++;
	VIPO_silent_counter++;
}

void Restaurant::AssigningCookToOrder(Order* ord,Cook* cook)
{
	
	cook->SetOrderAssignedTo(ord);
	ord->SetWaitingTime(timestep-ord->getArrivalTime());
	ord->setStatus(SRV);
	BusyCooks.enqueue(cook,-cook->getAvailabilityTime());
	OrdersInServing.enqueue(ord);
}

void Restaurant::WaitingOrdersToServed()
{
	Cook* CookAvailable;
	Order* ServingOrder;
	Order* PromotionOrder;
	Normal_OrdersWaiting.peekFront(PromotionOrder);
	int ServingTime;
	while(Normal_OrdersWaiting.peekFront(PromotionOrder) && (timestep-PromotionOrder->getArrivalTime()) == TimeBeforePromotion )
	{
		Normal_OrdersWaiting.dequeue(PromotionOrder);
		PromotionOrder->SetType(TYPE_VIP);
		WaitingOrderVIPenqueue(PromotionOrder);
		Normal_OrdersWaiting.peekFront(PromotionOrder);
		AutoPromoted_Count++;
	}
	bool urgent=true;
	while (urgent)
	{
		urgent=false;
		VIP_OrdersWaiting.peekFront(ServingOrder);
		if (VIP_OrdersWaiting.peekFront(ServingOrder) &&(timestep-ServingOrder->getArrivalTime()) == VIP_MaxWaitingTime)
		{
			Urgent_count++;
			urgent=true;
			ServingOrder=WaitingOrderVIPdequeue();
			ServingOrder->setStatus(SRV);
			if (VIP_AvailableCook.peekFront(CookAvailable))
			{
				VIP_AvailableCook.dequeue(CookAvailable);
				ServingTime=(int)ceil((double)ServingOrder->GetSize()/(double)CookAvailable->GetSpeed());
				ServingOrder->SetServTime(ServingTime);
				CookAvailable->SetAvailabilityTime(timestep+ServingTime);
				AssigningCookToOrder(ServingOrder,CookAvailable);
				O_waiting_count_VIP--;
				C_Available_count_VIP--;
			}
			else if (Normal_AvailableCook.peekFront(CookAvailable))
			{
				Normal_AvailableCook.dequeue(CookAvailable);
				ServingTime=(int)ceil((double)ServingOrder->GetSize()/(double)CookAvailable->GetSpeed());
				ServingOrder->SetServTime(ServingTime);
				CookAvailable->SetAvailabilityTime(timestep+ServingTime);
				AssigningCookToOrder(ServingOrder,CookAvailable);
				O_waiting_count_VIP--;
				C_Available_count_Normal--;
			}
			else if (Vegan_AvailableCook.peekFront(CookAvailable))
			{
				Vegan_AvailableCook.dequeue(CookAvailable);
				ServingTime=(int)ceil((double)ServingOrder->GetSize()/(double)CookAvailable->GetSpeed());
				ServingOrder->SetServTime(ServingTime);
				CookAvailable->SetAvailabilityTime(timestep+ServingTime);
				AssigningCookToOrder(ServingOrder,CookAvailable);
				O_waiting_count_VIP--;
				C_Available_count_Vegan--;
			}
			else if (CooksAtBreak.peekFront(CookAvailable))
			{
				CooksAtBreak.dequeue(CookAvailable);
				ServingTime=(int)ceil(((double)ServingOrder->GetSize() )/(double)CookAvailable->GetSpeed());
				ServingOrder->SetServTime(ServingTime);
				CookAvailable->SetAvailabilityTime(timestep+ServingTime);
				AssigningCookToOrder(ServingOrder,CookAvailable);
				O_waiting_count_VIP--;
			}
			else if (CooksAtRest.peekFront(CookAvailable))
			{
				CooksAtRest.dequeue(CookAvailable);
				CookAvailable->SetCookStatus(URGENT);
				ServingTime=(int)ceil(((double)ServingOrder->GetSize() )/(double)CookAvailable->GetSpeed());
				ServingOrder->SetServTime(ServingTime);
				CookAvailable->SetAvailabilityTime(timestep+ServingTime);
				AssigningCookToOrder(ServingOrder,CookAvailable);
				O_waiting_count_VIP--;
			}
			else
			{
				urgent=false;
			}
		}
	}
	while (VIP_OrdersWaitingPriorityQueue.peekFront(ServingOrder) && (VIP_AvailableCook.peekFront(CookAvailable) || Normal_AvailableCook.peekFront(CookAvailable) || Vegan_AvailableCook.peekFront(CookAvailable)))
	{
		ServingOrder=WaitingOrderVIPdequeuePriority();
		O_waiting_count_VIP--;
		if (VIP_AvailableCook.peekFront(CookAvailable))
		{
			VIP_AvailableCook.dequeue(CookAvailable);
			ServingTime=(int)ceil((double)ServingOrder->GetSize()/(double)CookAvailable->GetSpeed());
			ServingOrder->SetServTime(ServingTime);
			CookAvailable->SetAvailabilityTime(timestep+ServingTime);
			AssigningCookToOrder(ServingOrder,CookAvailable);
			C_Available_count_VIP--;
		}
		else if (Normal_AvailableCook.peekFront(CookAvailable))
		{
			Normal_AvailableCook.dequeue(CookAvailable);
			ServingTime=(int)ceil((double)ServingOrder->GetSize()/(double)CookAvailable->GetSpeed());
			ServingOrder->SetServTime(ServingTime);
			CookAvailable->SetAvailabilityTime(timestep+ServingTime);
			AssigningCookToOrder(ServingOrder,CookAvailable);
			C_Available_count_Normal--;
		}
		else if (Vegan_AvailableCook.peekFront(CookAvailable))
		{
			Vegan_AvailableCook.dequeue(CookAvailable);
			ServingTime=(int)ceil((double)ServingOrder->GetSize()/(double)CookAvailable->GetSpeed());
			ServingOrder->SetServTime(ServingTime);
			CookAvailable->SetAvailabilityTime(timestep+ServingTime);
			AssigningCookToOrder(ServingOrder,CookAvailable);
			C_Available_count_Vegan--;
		}		
	}
	while (Vegan_OrdersWaiting.peekFront(ServingOrder) && Vegan_AvailableCook.peekFront(CookAvailable))
	{
		Vegan_OrdersWaiting.dequeue(ServingOrder);
		Vegan_AvailableCook.dequeue(CookAvailable);
		ServingTime=(int)ceil((double)ServingOrder->GetSize()/(double)CookAvailable->GetSpeed());
		ServingOrder->SetServTime(ServingTime);
		CookAvailable->SetAvailabilityTime(timestep+ServingTime);
		AssigningCookToOrder(ServingOrder,CookAvailable);
		O_waiting_count_Vegan--;
		C_Available_count_Vegan--;
	}
	while (Normal_OrdersWaiting.peekFront(ServingOrder) && ( Normal_AvailableCook.peekFront(CookAvailable) || VIP_AvailableCook.peekFront(CookAvailable) ) )
	{
		Normal_OrdersWaiting.dequeue(ServingOrder);
		ServingTime=(int)ceil((double)ServingOrder->GetSize()/(double)CookAvailable->GetSpeed());
		ServingOrder->SetServTime(ServingTime);
		CookAvailable->SetAvailabilityTime(timestep+ServingTime);
		O_waiting_count_Normal--;
		if (Normal_AvailableCook.peekFront(CookAvailable))
		{
			Normal_AvailableCook.dequeue(CookAvailable);
			AssigningCookToOrder(ServingOrder,CookAvailable);
			C_Available_count_Normal--;
		}
		else if (VIP_AvailableCook.peekFront(CookAvailable))
		{
			VIP_AvailableCook.dequeue(CookAvailable);
			AssigningCookToOrder(ServingOrder,CookAvailable);
			C_Available_count_VIP--;
		}
	}

}
void Restaurant::MovingBreakToAvailable()
{
	Cook* In_BreakCook;
	while(CooksAtBreak.peekFront(In_BreakCook)&&In_BreakCook->getAvailabilityTime()==timestep)
	{
		CooksAtBreak.dequeue(In_BreakCook);
		In_BreakCook->SetAvailabilityTime(NULL);
		if(In_BreakCook->GetCookstatus()==URGENT)
		{
			In_BreakCook->setSpeed(In_BreakCook->GetSpeed()*2);
			In_BreakCook->SetCookStatus(SAFE);

		}
		MoveToAvailable(In_BreakCook);
	}
}
void Restaurant::MovingRestToAvailable()
{
	Cook* In_RestCook;
	while(CooksAtRest.peekFront(In_RestCook)&&In_RestCook->getAvailabilityTime()==timestep)
	{
		CooksAtRest.dequeue(In_RestCook);
		In_RestCook->SetAvailabilityTime(NULL);
		if(In_RestCook->GetCookstatus()==INJ)
		{
			In_RestCook->setSpeed(In_RestCook->GetSpeed()*2);
			In_RestCook->SetCookStatus(SAFE);

		}
		MoveToAvailable(In_RestCook);
	}
}
void Restaurant::CheckBusyCooks()
{
	 srand( (unsigned)time( NULL ) );
	Cook*BusyCook;
	double R;
	R=(double) rand()/RAND_MAX;
	if(BusyCooks.peekFront(BusyCook))
	{
		if(R<=InjuryProbability&&BusyCook->GetCookstatus()==SAFE)
	{
		BusyCook->setSpeed(BusyCook->GetSpeed()/2);
		BusyCook->SetCookStatus(INJ);
		inj_C++;
		int remainaingTimetoCompelete=BusyCook->getAvailabilityTime()-timestep;
		BusyCooks.dequeue(BusyCook);
		BusyCook->SetAvailabilityTime((remainaingTimetoCompelete*2+timestep));
		BusyCooks.enqueue(BusyCook,-BusyCook->getAvailabilityTime());
	}
	}
		while(BusyCooks.peekFront(BusyCook)&&BusyCook->getAvailabilityTime()==timestep)
		{
			BusyCooks.dequeue(BusyCook);
			finished_orders++;
			SRV_to_Finshed(BusyCook->GetOrderAssignedTo());//remove From Served to finished
			BusyCook->SetOrderAssignedTo(nullptr);
			if(BusyCook->GetCookstatus()==INJ)
			{
				CooksAtRest.enqueue(BusyCook);
			}
			else
			{
			if(BusyCook->GetNo_SER_ORD()==BusyCook->getOrdersBeforeBreak())
			{
				BusyCook->SetAvailabilityTime(BusyCook->GetBreakDuration()+timestep);
				CooksAtBreak.enqueue(BusyCook,-BusyCook->getAvailabilityTime());
			}
			else
			{
				BusyCook->SetAvailabilityTime(NULL);
				MoveToAvailable(BusyCook);
			}
			}
		}
	
}
void Restaurant::assignmentfunction()
{

	MovingBreakToAvailable();
	MovingRestToAvailable();
	CheckBusyCooks();
	WaitingOrdersToServed();
}
void Restaurant::SRV_to_Finshed(Order*finished)
{
	int ID=finished->GetID();
	Queue<Order*> temp;
	Order*tempOrd;
	while(OrdersInServing.dequeue(tempOrd))
	{
		if(tempOrd->GetID()==ID)
		{
			tempOrd->setStatus(DONE);
			OrdersFinished.enqueue(tempOrd);
		}
		else
		{
			temp.enqueue(tempOrd);
		}
	} 
	while(temp.dequeue(tempOrd))
	{
		OrdersInServing.enqueue(tempOrd);
	}
}
void Restaurant::MoveToAvailable(Cook* AvailableCook)
{
	if(AvailableCook->GetType()==TYPE_VIP)
		{
			VIP_AvailableCook.enqueue(AvailableCook);
			C_Available_count_VIP++;

		}
		else if(AvailableCook->GetType()==TYPE_NRM)
		{
			Normal_AvailableCook.enqueue(AvailableCook);
			C_Available_count_Normal++;
		}
		else
		{
			Vegan_AvailableCook.enqueue(AvailableCook);
			C_Available_count_Vegan++;
		}
}

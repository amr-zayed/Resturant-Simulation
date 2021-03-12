#ifndef __RESTAURANT_H_
#define __RESTAURANT_H_

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include "..\Events\Event.h"
#include"..\Generic_DS\PriorityQueue.h"
#include <fstream>

#include "..\PriorityDESqueue.h"
#include "Order.h"

// it is the maestro of the project
class Restaurant
{
private:
	GUI* pGUI;
	Queue<Event*> EventsQueue;	//Queue of all events that will be loaded from file


	///////////////////////////ADDED/////////////////////
	int TimeBeforePromotion;   //is the time for the normal order will wait until promotion
	int timestep;           
	int arrived_orders;       
	int finished_orders;      
	int canceled_orders;     
	int promoted_orders;
	int VIP_MaxWaitingTime;		//Max time spent in the vip queue
	double InjuryProbability;
	int CookRestPeriodInjury;
	int AutoPromoted_Count;
	int Urgent_count;
	int inj_C;
	string print;
	int totalservVIP;
	int totalservNRM;
	int totalservVGAN;
	ifstream file;
	PriorityQueue<Order*> VIP_OrdersWaitingPriorityQueue; 
	Queue<Order*> VIP_OrdersWaiting;  //VIP orders in waiting must be assigned to cook if any
	Queue<Order*> Normal_OrdersWaiting;   //Normal orders in waiting assigned to vip and normal cooks after vip is served
	Queue<Order*> Vegan_OrdersWaiting;    //Vegan orders in waiting assignd to vegan cook after vip is served


	Queue<Order*> OrdersInServing;  //All orders that are currently assigned to cooks
	Queue<Order*> OrdersFinished;  //All orders that are served


	Queue<Cook*>  VIP_AvailableCook;  //Vip cooks that are ready to be assigned to orders
	Queue<Cook*>  Normal_AvailableCook; //Normal cooks that are ready to be assigned to orders
	Queue<Cook*>  Vegan_AvailableCook;  //Vegan cooks that are ready to be assigned to orders


	PriorityQueue<Cook*>  BusyCooks;  //All cooks assigned to cooks
	
	Queue<Cook*> CooksAtRest;	//Cooks that are injuried
	PriorityQueue<Cook*>  CooksAtBreak;
	int O_waiting_count_VIP;//gives the count of the waiting VIP orders
	int O_waiting_count_Normal; //gives the count of the waiting Normal orders
	int O_waiting_count_Vegan; //gives the count of the waiting Vegan orders
	int C_Available_count_VIP; //gives the count of the available VIP cooks
	int C_Available_count_Normal; //gives the count of the available Normal cooks
	int C_Available_count_Vegan; //gives the count of the available Vegan cooks

	int VIPO_silent_counter;
	int NormO_silent_counter;
	int VegO_silent_counter;
	int VIPC_silent_counter;
	int NormC_silent_counter;
	int VegC_silent_counter;

	void MoveToAvailable(Cook* AvailableCook);// moves Cook to available cooks lists
	void MovingBreakToAvailable();//Moves Cooks in breakList to Available Cooklist After finishing their break
	void MovingRestToAvailable();//Moves Cooks in RestList to Available Cooklist After finishing their Rest
	void CheckBusyCooks();//Checks Busycook list if is it the cookbreak time Then moves it to break-list if is injured move it to rest-list if none of the previous then move it to Avilable cook 
	void SRV_to_Finshed(Order*);//Moves the order from Order in serving List TO Finished Orders List


	void WaitingOrdersToServed();
	Order* WaitingOrderVIPdequeue();  //Removes VIP orders from the waiting and returns removed order
	Order* WaitingOrderVIPdequeuePriority();  
	void AssigningCookToOrder(Order*,Cook*, string&);
	void OutputFunction();
	void SimMODE(int);
	void assignmentfunction();
public:

	Restaurant();
	~Restaurant();
	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	void RunSimulation();

	void FillDrawingList();

	//ADDED MEMBER FUNCTIONS TO ENQUEUE IN THE NEWELLY CREATED QUEUES
	void AddtoNormal_OrdersWaitingQueue(Order* ord); //enqueues the order in the Normal waiting queue
	void AddtoVegan_OrdersWaitingQueue(Order* ord); //enqueues the order in the Vegan waiting list
	void WaitingOrderVIPenqueue(Order*);  //adds the vip order to the queues
	void promotion(int id, double);
	void LoadingFunction();
	void CancelOrder(int id);
};

#endif
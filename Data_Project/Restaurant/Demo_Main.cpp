//#include "Drawing.h"
#include "Rest\Restaurant.h"
#include "GUI\GUI.h"
#include"Generic_DS\PriorityQueue.h"

int main()
{
	
	Restaurant* pRest = new Restaurant;
	pRest->RunSimulation();
	delete pRest;
	
	return 0;
}

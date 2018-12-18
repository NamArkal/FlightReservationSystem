#include "kernel.h"
#include "main.h"
#include "thread.h"
#include "request.h"
#include "list.h"

List<Thread *> *grantedE = new List<Thread *>();
List<Thread *> *grantedB = new List<Thread *>();
List<Thread *> *grantedC = new List<Thread *>();
List<Thread *> *grantedD = new List<Thread *>();
List<Request*> *denied = new List<Request*>();
List<Request*> *allGranted = new List<Request*>();
Bitmap *map = new Bitmap(20);

void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < which; num++) {
        printf("*** thread %d looped %d times\n", which, num);
	// ---- Lab Assignment 1 ---
        kernel->currentThread->Yield();
    }
}

bool isReqComplete(int source, Request* r){
    return source < r->getDestCity();
}

bool atDestination(int source, Request* r){
    return source == r->getDestCity();
}

char city(int source){
    char res;
    switch(source){
        case 0: res = 'A';
            break;
        case 1: res = 'B';
            break;
        case 2: res = 'C';
            break;
        case 3: res = 'D';
            break;
        case 4: res = 'E';
            break;
        default: std::cout << "Error\n";
    }
    return res;
}

void reservationThreadImpl(int source){
    std::cout << "\n----- New reservation request generated\n";
    Request *r = new Request(source);
    std::cout << "The request Id: " << r->getReqNumber() << ", source city: " << city(r->getSourceCity()) << ", destination city: " << city(r->getDestCity())
              << ", number of passengers: " << r->getNumberOfPassengers() << "\n";

    int clearBits = map->NumClear();
    std::cout << "Available seats on flight are: " << clearBits << "\n";

    if(clearBits >= r->getNumberOfPassengers()){
	std::cout << "*** Request accepted. Seat numbers assigned are: " << "\n";
        for(int i=0; i<r->getNumberOfPassengers(); i++){
            int index = map->FindAndSet();
            r->seatNumbers->Append(index);
	    std::cout << index << "\n";
        }
	std::cout << "Available seats after assignment: " << map->NumClear() << "\n";
	allGranted->Append(r);
        int dest = r->getDestCity();
        switch(dest){
            case 1: grantedB->Append(kernel->currentThread);
                break;
            case 2: grantedC->Append(kernel->currentThread);
                break;
            case 3: grantedD->Append(kernel->currentThread);
		break;
            case 4: grantedE->Append(kernel->currentThread);
                break;
	    default : std::cout << "Error.\n";
        }
       	kernel->currentThread->Sleep(false);
	ListIterator<int> li = r->getSeatNumbers();
    	//deallocate bitmap
    	while(!li.IsDone()){
        	map->Clear(li.Item());
		std::cout << "Deallocating seat " << li.Item() << "\n";
      		li.Next();
    	}
    //thread.finish
    kernel->currentThread->Finish();
    } else {
	std::cout << "\n*** Request denied due to full flight\n";
        denied->Append(r);
        kernel->currentThread->Finish();
    }
}

void flightThreadImpl(){
    
    std::cout << "\n\n***** Implementing Flight Simulation *****\n\n";
    //int count = 0;
    int nextDest = 0;
    for(int i=0; i<5; i++) {
        //for(int j=0; j<5; j++) {
	//if(i != 4){
            //Thread *t1 = new Thread((char*)i+j);
            //t1->Fork((VoidFunctionPtr) reservationThreadImpl, (void *) i);
	    //thread.yield
	    //kernel->currentThread->Yield();
        //}
	//}
        //print current city
        std::cout << "\n\n----- At city " << city(i) << " -----\n";
	//print remaining requests in granted and no. of seats taken
        ListIterator<Request *> *lrs1 = new ListIterator<Request *>(allGranted);
        while (!lrs1->IsDone()) {
            if (atDestination(i, lrs1->Item())) {
                std::cout << "Request " << lrs1->Item()->getReqNumber() << " is getting off. Seats returned are ";
                ListIterator<int> seats = lrs1->Item()->seatNumbers;
                while (!seats.IsDone()) {
                    std::cout << seats.Item() << "\t";
                    seats.Next();
                }
                std::cout << "\n";
            }
            lrs1->Next();
        }
        ListIterator<Request *> *lrs2 = new ListIterator<Request *>(allGranted);
        while (!lrs2->IsDone()) {
            if (isReqComplete(i, lrs2->Item()))
                std::cout << "Request " << lrs2->Item()->getReqNumber() << " is still on plane\n";
            lrs2->Next();
        }
        //wake up reservation thread whose city is B and make it ready to run
	if(i>0)
        switch(i) {
            case 1: {ListIterator<Thread *> *liB = new ListIterator<Thread *>(grantedB);
                while(!liB->IsDone()){
                    Thread *t = liB->Item();
                    kernel->scheduler->ReadyToRun(t);
                    liB->Next();
                }}
                break;
            case 2: {ListIterator<Thread *> *liC = new ListIterator<Thread *>(grantedC);
                while(!liC->IsDone()) {
                    Thread *t = liC->Item();
                    kernel->scheduler->ReadyToRun(t);
                    liC->Next();
                }}
                break;
            case 3: {ListIterator<Thread *> *liD = new ListIterator<Thread *>(grantedD);
                while(!liD->IsDone()){
                    Thread *t = liD->Item();
                    kernel->scheduler->ReadyToRun(t);
                    liD->Next();
                }}
                break;
            case 4: {ListIterator<Thread *> *liE = new ListIterator<Thread *>(grantedE);
                while(!liE->IsDone()){
                    Thread *t = liE->Item();
                    kernel->scheduler->ReadyToRun(t);
                    liE->Next();
                }}
                break;
	    default : std::cout << "Error.\n";
        }

	for(int j=0; j<5; j++) {
        if(i != 4){
            Thread *t1 = new Thread((char*)i+j);
            t1->Fork((VoidFunctionPtr) reservationThreadImpl, (void *) i);
            //thread.yield
            kernel->currentThread->Yield();
        }
        }
	std::cout << "\nOccupancy rate is " << 20-(map->NumClear()) << "/20\n\n";
	//put current flight thread to yield
        if(i==4){
		// print discarded request Ids
    		ListIterator<Request *> *deniedLi = new ListIterator<Request *>(denied);
    		std::cout << "\n\nThe denied requests are: ";
    		while (!deniedLi->IsDone()) {
        		std::cout << deniedLi->Item()->getReqNumber() << "\t";
        		deniedLi->Next();
   		 }
		std::cout << "\n\n";
		kernel->currentThread->Finish();
        } else
		kernel->currentThread->Yield();
    }
}

void
ThreadTest()
{
  Thread *t = new Thread("forked flight thread");
  t->Fork((VoidFunctionPtr) flightThreadImpl, (void *) 1);
  kernel->currentThread->Yield();
}

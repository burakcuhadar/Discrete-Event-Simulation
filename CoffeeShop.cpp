#include "CoffeeShop.h"
#include <iostream>



CoffeeShop::CoffeeShop(int N){
    this-> N = N;
    //initialize cashiers
    cashiers.reserve(N);
    for(int i=0; i< N; i++) {
        Worker aWorker;
        cashiers.push_back( aWorker );
    }
    //initialize baristas
    baristas.reserve(N/3);
    for(int i=0, n=N/3; i< n; i++) {
        Worker aWorker;
        baristas.push_back( aWorker );
    }
}

void CoffeeShop::runSimulation1(std::vector<Order> & orders) {

    //push every order to the event queue as an arrival event
    for(int i=0, m=orders.size(); i<m; i++) {
    	Event tmpEvent(-1, orders[i].arrivalTime, EventType::arrival, &orders[i]);
    	eventQueue.push(tmpEvent);
    }

    while( !eventQueue.empty() ) {

    	//get information about the event that has happened
        Event currentEvent = eventQueue.top();
        Order* currentOrder = currentEvent.eventOrder;
        int currentIndex = currentEvent.index;
        //update the time
        currentTime = currentEvent.time;
        //since the event at the top of the queue has passed we can delete it
        eventQueue.pop();

        if(currentEvent.type == EventType::take_order) {   //order has taken by cashier

        	//update cashier's information
            cashiers[currentIndex].busyTime += currentOrder->orderTime;
            cashiers[currentIndex].isBusy = false;
            //search for an available barista to brew the order
            int availableBarista = getAvailableBarista();
            if(availableBarista == -1) {	//available barista cannot be found
            	//wait in barista queue
                baristaQueue.push(currentOrder);
                //update maximum barista queue length if necessary
                int length = baristaQueue.size();
                if(length > maxBarQueLength) {
                    maxBarQueLength = length;
                }
            }else {							
            	//there is an available barista make it brew the order
                baristas[availableBarista].isBusy = true;
                //create and push the event for brewing by barista, brewing process has begun
                Event tmpEvent(availableBarista, currentTime + currentOrder->brewTime, EventType::brew, currentOrder);
                eventQueue.push(tmpEvent);
            }

            //now there is an available cashier, when there is an order waiting in cashier queue, push it to the eventQueue 
            if(!cashierQueue.empty()) {
                    cashiers[currentIndex].isBusy = true;
                    Order* queueOrder = cashierQueue.front();
                    Event tmpEvent(currentIndex, currentTime+queueOrder->orderTime, EventType::take_order, queueOrder );
                    eventQueue.push(tmpEvent);
                    cashierQueue.pop();
            }

        }else if(currentEvent.type == EventType::brew) {   //order has brewed by barista

        	//update barista's information
            baristas[currentIndex].busyTime += currentOrder->brewTime;
            baristas[currentIndex].isBusy = false;
            //coffee is done, update its turnaround time
            currentOrder->turnaroundTime = currentTime - currentOrder->arrivalTime;

            //now there is an available barista, when there is an order waiting in barista queue, push it to the event queue
            if(!baristaQueue.empty()) {
                baristas[currentIndex].isBusy = true;
                Order* queueOrder = baristaQueue.top();
                Event tmpEvent(currentIndex, currentTime+queueOrder->brewTime, EventType::brew, queueOrder);
                eventQueue.push(tmpEvent);
                baristaQueue.pop();
            }

        }else if(currentEvent.type == EventType::arrival) { // a new order has arrived at the coffee shop

        	//search for an available cashier
            int availableCashier = getAvailableCashier();
            if(availableCashier == -1){		//no available cashier
            	//wait in cashier queue
                cashierQueue.push(currentOrder);
                //update the maximum cashier queue length if necessary
                int length = cashierQueue.size();
                if(length > maxCashQueLength) {
                    maxCashQueLength = length;
                }
            }else {
            	//cashier has begun taking order from a customer
                cashiers[availableCashier].isBusy = true;
                //create and push the event for taking order by cashier
                Event tmpEvent(availableCashier, currentTime + currentOrder->orderTime, EventType::take_order, currentOrder);
                eventQueue.push(tmpEvent);
            }

        }
    }
}

void CoffeeShop::runSimulation2(std::vector<Order> & orders){
    resetSimulation();
    //create N/3 barista queues
    baristaQueue2.reserve(N/3);
    maxBarQueLength2.reserve(N/3);
    for(int i=0, n= N/3; i<n; i++){
        std::priority_queue<Order*, std::vector<Order*>, Comp> tmpQueue;
        baristaQueue2.push_back(tmpQueue);
        maxBarQueLength2.push_back(0);
    }

    //push every order to the event queue as an arrival event
    for(int i=0, m=orders.size(); i<m; i++) {
        Event tmpEvent(-1, orders[i].arrivalTime, EventType::arrival, &orders[i]);
        eventQueue.push(tmpEvent);
    }

    while( !eventQueue.empty() ) {
        //get information about the event that has happened
        Event currentEvent = eventQueue.top();
        Order* currentOrder = currentEvent.eventOrder;
        int currentIndex = currentEvent.index;
        //update the time
        currentTime = currentEvent.time;
        //since the event at the top of the queue has passed we can delete it
        eventQueue.pop();

        if(currentEvent.type == EventType::take_order) {   //order has taken by cashier

            //update cashier's information
            cashiers[currentIndex].busyTime += currentOrder->orderTime;
            cashiers[currentIndex].isBusy = false;
            
            //send the order to the barista, if it is not available wait in the queue
            int barQueIndex = currentIndex / 3;
            if(baristas[barQueIndex].isBusy) {    //barista is not available
                //wait in barista queue
                baristaQueue2[barQueIndex].push(currentOrder);
                //update maximum barista queue length if necessary
                int length = baristaQueue2[barQueIndex].size();
                if( length > maxBarQueLength2[barQueIndex] ) {
                    maxBarQueLength2[barQueIndex] = length;
                }
            }else {                         
                //barista is available, make it brew the order
                baristas[barQueIndex].isBusy = true;
                //create and push the event for brewing by barista, brewing process has begun
                Event tmpEvent(barQueIndex, currentTime + currentOrder->brewTime, EventType::brew, currentOrder);
                eventQueue.push(tmpEvent);
            }

        }else if(currentEvent.type == EventType::brew) {   //order has brewed by barista

            //update barista's information
            baristas[currentIndex].busyTime += currentOrder->brewTime;
            baristas[currentIndex].isBusy = false;
            //coffee is done, update its turnaround time
            currentOrder->turnaroundTime = currentTime - currentOrder->arrivalTime;

        }else if(currentEvent.type == EventType::arrival) { // a new order has arrived at the coffee shop

            //search for an available cashier
            int availableCashier = getAvailableCashier();
            if(availableCashier == -1){     //no available cashier
                //wait in cashier queue
                cashierQueue.push(currentOrder);
                //update the maximum cashier queue length if necessary
                int length = cashierQueue.size();
                if(length > maxCashQueLength) {
                    maxCashQueLength = length;
                }
            }else {
                //cashier has begun taking order from a customer
                cashiers[availableCashier].isBusy = true;
                //create and push the event for taking order by cashier
                Event tmpEvent(availableCashier, currentTime + currentOrder->orderTime, EventType::take_order, currentOrder);
                eventQueue.push(tmpEvent);
            }

        }

        //when there is an order waiting in cashier queue, push it to the eventQueue if there is an available cashier
        if(!cashierQueue.empty()) {
            int availableCashier = getAvailableCashier();
            if(availableCashier != -1){
                cashiers[availableCashier].isBusy = true;
                Order* queueOrder = cashierQueue.front();
                Event tmpEvent(availableCashier, currentTime+queueOrder->orderTime, EventType::take_order, queueOrder );
                eventQueue.push(tmpEvent);
                cashierQueue.pop();
            }
        }

        //when there is an order waiting in a barista queue, push it to the event queue if there is an available barista
        for(int i=0, n=N/3; i<n; i++) {
            if(!baristaQueue2[i].empty()) {
                if(!baristas[i].isBusy) {
                    baristas[i].isBusy = true;
                    Order* queueOrder = baristaQueue2[i].top();
                    Event tmpEvent(i, currentTime+queueOrder->brewTime, EventType::brew, queueOrder);
                    eventQueue.push(tmpEvent);
                    baristaQueue2[i].pop();
                }
            }
        }
    }
}

//returns the index of the first non-busy cashier, returns -1 if there is none
int CoffeeShop::getAvailableCashier() const{
    for(int i=0; i<N; i++) {
        if(! cashiers[i].isBusy) {
            return i;
        }
    }
    return -1;
}

//returns the index of the first non-busy barista, returns -1 if there is none
int CoffeeShop::getAvailableBarista() const{
    int n = N/3;
    for(int i=0; i<n; i++) {
        if(! baristas[i].isBusy) {
            return i;
        }
    }
    return -1;
}

void CoffeeShop::resetSimulation() {
    //reset cashiers
    for(int i=0; i < N; i++) {
        cashiers[i].busyTime = 0.0;
        cashiers[i].isBusy = false;
    }
    //reset baristas
    for(int i=0, n= N/3; i < n; i++) {
        baristas[i].busyTime = 0.0;
        baristas[i].isBusy = false;
    }
    //reset time and max cashier queue length
    currentTime = 0.0;
    maxCashQueLength = 0;
}

CoffeeShop::~CoffeeShop() {}

int CoffeeShop::getMaxBarQueLength() const { return maxBarQueLength; }

int CoffeeShop::getMaxCashQueLength() const { return maxCashQueLength; }

double CoffeeShop::getCurrentTime() const { return currentTime; }

std::vector<Worker> CoffeeShop::getBaristas() const { return baristas; }

std::vector<Worker> CoffeeShop::getCashiers() const { return cashiers; }

std::vector<int> CoffeeShop::getMaxBarQueLength2() const { return maxBarQueLength2; }

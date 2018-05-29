#ifndef PROJECT2_COFFEESHOP_H
#define PROJECT2_COFFEESHOP_H

#include "Order.h"
#include "Worker.h"
#include "Event.h"
#include <vector>
#include <queue>

struct Comp{
    bool operator()(const Order* lhs, const Order* rhs) {
        return lhs->price < rhs->price;
    }
};

struct Comp2{
    bool operator()(const Event& lhs, const Event& rhs) {
        return lhs.time > rhs.time;
    }
};

class CoffeeShop
{
private:
    int N;	//number of cashiers
    double currentTime = 0.0;
    int maxCashQueLength = 0;
    int maxBarQueLength = 0;            //for model 1
    std::vector<int> maxBarQueLength2;  //for model 2 
    std::vector< Worker > cashiers;
    std::vector< Worker > baristas;
    std::queue<Order*> cashierQueue;
    std::priority_queue<Order*, std::vector<Order*>, Comp> baristaQueue;                //for model 1
    std::vector<std::priority_queue<Order*, std::vector<Order*>, Comp>> baristaQueue2;   //for model 2
    std::priority_queue<Event, std::vector<Event>, Comp2> eventQueue;
    int getAvailableCashier() const;
    int getAvailableBarista() const;
    void resetSimulation();
public:
    CoffeeShop(int N);	//N = number of cashiers
    ~CoffeeShop();
    void runSimulation1(std::vector<Order> & orders);
    void runSimulation2(std::vector<Order> & orders);
    //getters
    double getCurrentTime() const;
    int getMaxCashQueLength() const;
    int getMaxBarQueLength() const;
    std::vector<Worker> getCashiers() const;
    std::vector<Worker> getBaristas() const;
    std::vector<int> getMaxBarQueLength2() const;

};

#endif //PROJECT2_COFFEESHOP_H

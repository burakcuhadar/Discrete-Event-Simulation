#include "Order.h"

Order::Order(double arrivalTime, double orderTime, double brewTime, double price) {
    this->arrivalTime = arrivalTime;
    this->orderTime = orderTime;
    this->brewTime = brewTime;
    this->price = price;
}

Order::~Order() {}

Order::Order() {}


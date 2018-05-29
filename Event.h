#ifndef PROJECT2_EVENT_H
#define PROJECT2_EVENT_H

#include "Order.h"

enum class EventType{
    take_order,
    brew,
    arrival,
};

class Event{
public:
    int index;
    double time;
    Order* eventOrder = nullptr;	//the order that is effected by the event
    EventType type;
    Event(int index, double time, EventType type, Order* eventOrder);
    Event(const Event& other);
};
#endif //PROJECT2_EVENT_H

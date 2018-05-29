#include "Event.h"

Event::Event(int index, double time, EventType type, Order*  eventOrder) {
    this->eventOrder = eventOrder;
    this->index = index;
    this->time  = time;
    this->type = type;
}

Event::Event(const Event &other) {
    eventOrder = other.eventOrder;
    index = other.index;
    time  = other.time;
    type = other.type;
}

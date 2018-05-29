#ifndef PROJECT2_ORDER_H
#define PROJECT2_ORDER_H
class Order
{
public:
    double arrivalTime = 0.0;
    double orderTime = 0.0;
    double brewTime = 0.0;
    double price = 0.0;
    double turnaroundTime = 0.0;
    Order(double arrivalTime, double orderTime, double brewTime, double price);
    Order();
    ~Order();

};
#endif //PROJECT2_ORDER_H

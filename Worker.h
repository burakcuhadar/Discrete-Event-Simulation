#ifndef PROJECT2_WORKER_H
#define PROJECT2_WORKER_H
class Worker{
public:
    double busyTime = 0.0;
    bool isBusy = false;
    Worker();
    Worker(const Worker& other);
};
#endif //PROJECT2_WORKER_H

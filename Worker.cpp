#include "Worker.h"

Worker::Worker() {}

Worker::Worker(const Worker &other) {
    busyTime = other.busyTime;
    isBusy = other.isBusy;
}
// Compile as: g++ poolTester.cc tPool.cc -std=c++20 -pthread -o poolTester
//then use .\poolTester.exe on windows or ./poolTester on linux
// Note we want the 2020 standard to use this version of semaphores

#include <iostream>
#include "tPool.h"

typedef struct fooParm {
    int id;
    int waitTime;
} fooParm;

void foo(void* parm) {
    fooParm* p = (fooParm*)parm; // downcast
    
  
    std::string output = "  Foo called with id " + std::to_string(p->id) + "\n";
    std::cout << output;
    std::this_thread::sleep_for(std::chrono::milliseconds(p->waitTime));
}

int main() {
    srand(time(NULL));  // each run will be different

    tPool* tp = new tPool(4);

    for (int i = 0; i < 20; ++i) {
        fooParm* p = new fooParm;
        p->id = i;
        p->waitTime = rand() % 1000;
        tp->addWork(foo, p);
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));
    tp->stopPool();
    delete tp;

    return 0;
}

#ifndef TPOOL_H
#define TPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <semaphore>
#include <mutex>

class tPool {
public:
    tPool(int numThreads);
    ~tPool();
    void addWork(std::function<void(void*)> work, void* param);
    void stopPool();

private:
    void workerThread(int id);

    int numThreads;
    std::vector<std::thread*> threads;
    std::queue<std::function<void(void*)>> workQueue;
    std::queue<void*> paramQueue;
    std::binary_semaphore* qLock;
    std::counting_semaphore<1>* workAvailable;
    std::binary_semaphore* stopSemaphore;
    bool stop;
    std::mutex queueMutex;
};

#endif // TPOOL_H
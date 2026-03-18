#ifndef TPOOL_H
#define TPOOL_H

#include <thread>
#include <semaphore> 
#include <chrono>
#include <queue>
#include <vector>
#include <iostream>


const unsigned int QUEUE_SIZE = 100;
const unsigned int DEFAULT_THREADS = 4;

class tPool
{
    private:
        std::binary_semaphore* qLock;
        std::counting_semaphore<QUEUE_SIZE>* qCount;
        std::queue<void*> argQueue;
        std::queue<void (*)(void*)> funcQueue;
        std::vector<std::thread*> ths;
        bool isDone;

        static void workerThread (tPool* tp, int id);
        static void empty(void* x);

    public:
        tPool (int numThreads);
        void addWork (void func(void* x), void* x);
        void stopPool ();
};

#endif
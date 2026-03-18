#include "tPool.h"

void tPool::workerThread (tPool* tp, int id)
{
    printf("Worker %d started\n", id);
    while(true)
    {
        tp->qCount->acquire();
        tp->qLock->acquire();
        printf("Worker %d about to do work\n", id);
        void* func = (void*) tp->funcQueue.front();
        void* arg = tp->argQueue.front();
        tp->funcQueue.pop();
        tp->argQueue.pop();
        tp->qLock->release();
        reinterpret_cast<void (*)(void*)>(func)(arg);
        if(tp->isDone)
        {
            break;
        }
    }
    printf("Worker %d stopping\n", id);
}

void tPool::empty (void* x)
{

}

tPool::tPool (int numThreads)
{
    this->isDone = false;
    this->qLock = new std::binary_semaphore(1);
    this->qCount = new std::counting_semaphore<QUEUE_SIZE>(0);

    for(int i = 0; i < numThreads; i++)
    {
        std::thread* th = new std::thread(tPool::workerThread, this, i);
        this->ths.push_back(th);
    }
}

void tPool::addWork(void func(void* x), void* x)
{
    printf("Adding Work\n");
    this->qLock->acquire();
    this->funcQueue.push(func);
    this->argQueue.push(x);
    this->qCount->release();
    this->qLock->release();
}

void tPool::stopPool()
{
    while (this->funcQueue.size()>0){} //Wait for no work left //Aquire a semaphore that can only exist when the queue is empty
    this->isDone = true;

    for(int i = 0; i < this->ths.size(); i++)
    {
        tPool::addWork(tPool::empty, NULL); //isDone is checked at the end as the thread hangs on qLock->acquire()
    }
    for(int i = 0; i < this->ths.size(); i++)
    {
        ths[i]->join();
        delete ths[i];
        ths.erase(ths.begin() + i);
        i--;
    }

    delete qLock;
    delete qCount;
}
//compile like g++ -std=c++11 -pthread primeFinder.cc -o primeFinder
#include <iostream>
#include <chrono>
#include <thread>

bool isPrime(int n) {
    if (n <= 1) {
        return false;
    }
    for (int i = 2; i < n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

void countPrimes(int start, int end, int threadNumber, int* result) {
    int count = 0;
    for (int i = start; i <= end; i++) {
        if (isPrime(i)) {
            count++;
        }
    }
    result[threadNumber] = count;
}

int main() {
/*
Original prime code
    // Ask user for a number
    int userInput;
    std::cout << "Enter a number: ";
    std::cin >> userInput;

    if (isPrime(userInput)) {
        std::cout << userInput << " is a prime number\n";
    } else {
        std::cout << userInput << " is not a prime number\n";
    }
*/
    // Ask user for a range
    int userRange1, userRange2;
    std::cout << "Enter a range: ";
    std::cin >> userRange1 >> userRange2;

    int rangeSize = userRange2 - userRange1;

    int numThreads = 4;

    int* totalPrimes = (int*)malloc(numThreads*sizeof(int));
    //astrix is a pointer, so it is a memory address
    
    // Start the timer
    auto start = std::chrono::high_resolution_clock::now();

    //number of threads existing
    std::thread* ths[numThreads];
    //ths is an array of the threads

    for (int i = 0; i < numThreads; i++) {
        //start and stop are the total range, divided evenly by the number of threads, and spaced based on which thread they are on
        //if there are 4 threads, and a range of 16, then ths[1] = 1,2,3,4, ths[2] = 5,6,7,8, and so on
        int start = (rangeSize / numThreads) * i;
        int stop = (rangeSize / numThreads) * (i + 1);

        //the actual line of code to make the thread
        std::thread* th = new std::thread(countPrimes, start, stop, i , totalPrimes);
        //ths[i] is the thread, and th is the thread that is being made
        //that comment is from copilot and i am not sure about it being correct
        ths[i] = th;
    }

    for (int i = 0; i<numThreads; i++) {
        //the join function is a dynamic timer that waits until every thread is finished running
        ths[i]->join();
    }
    
    int primeSum = 0;
    //sum up every thread's result into primeSum
    for (int i = 0; i < sizeof(totalPrimes); i++) {
        primeSum += totalPrimes[i];
    }

    //print function
    std::cout << "There are " << primeSum << " prime numbers between " << userRange1 << " and " << userRange2 << "\n";

    // End the timer
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time taken to execute main: " << elapsed.count() << " seconds\n";

    return 0;
}
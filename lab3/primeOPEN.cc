#include <iostream>
#include <chrono>
#include <omp.h>
#include <mutex>
//run with g++ primeOpen.cc -fopenmp -o primeOpen
//then ./primeOpen

std::mutex screenLock;

// Function to check if a number is prime
bool isPrime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}
// Function to count the number of primes in a range
int countPrimes(int start, int end) {
    int count = 0;
    for (int i = start; i <= end; i++) {
        if (isPrime(i)) {
            count++;
        }
    }
    return count;
}

// Blocking partitioning technique
int countPrimesBlocking(int start, int end) {

    int totalPrimes = 0;

    int numThreads = 5;

    int range = end - start;

    int numbersChecked = range / numThreads;

    omp_set_num_threads(numThreads);

    #pragma omp parallel reduction (+:totalPrimes)
    {
        int tn = omp_get_thread_num();
        int numT = omp_get_num_threads();
        screenLock.lock();
        std::cout << "I'm thread " << tn << " of " << numT << "\n";
        screenLock.unlock();
    
        //start timer
        auto start1 = std::chrono::high_resolution_clock::now();
    //the number after static is the size of the range that each thread will get
    #pragma omp for schedule (static, numbersChecked) //nowait
        for (int i=start; i<=end; i++) {
            if(isPrime(i)) {
                totalPrimes++;
            }
        }   //barrier (not a join one)
    // End the timer
    auto end1 = std::chrono::high_resolution_clock::now();
    //calculate time between
    std::chrono::duration<double> elapsed1 = end1 - start1;
    std::cout << "Time taken to execute Thread "<< tn << ": " << elapsed1.count() << " seconds\n";
    } //barrier (join)
return totalPrimes;

std::cout << "world\n";
}

int countPrimesStriping(int start, int end) {
    int totalPrimes = 0;

    int numThreads = 5;

    //int range = end - start;

    int numbersChecked = 1;

    omp_set_num_threads(numThreads);

    #pragma omp parallel reduction (+:totalPrimes)
    {
        int tn = omp_get_thread_num();
        int numT = omp_get_num_threads();
        screenLock.lock();
        std::cout << "I'm thread " << tn << " of " << numT << "\n";
        screenLock.unlock();

        //start timer
        auto start2 = std::chrono::high_resolution_clock::now();
    //the number after static is the size of the range that each thread will get
    #pragma omp for schedule (static, numbersChecked) //nowait
        for (int i=start; i<=end; i++) {
            if(isPrime(i)) {
                totalPrimes++;
            }
        }   //barrier (not a join one)
        // End the timer
        auto end2 = std::chrono::high_resolution_clock::now();
        //calculate time between
        std::chrono::duration<double> elapsed2 = end2 - start2;
        std::cout << "Time taken to execute Thread "<< tn << ": " << elapsed2.count() << " seconds\n";
    } //barrier (join)
return totalPrimes;

std::cout << "world\n";
}


/*
omp_set_num_threads(5);


// Striping partitioning technique
int countPrimesStriping(int start, int end, int numThreads) {
    int totalPrimes = 0;
    int primesPerThread[numThreads];
    double startTime = omp_get_wtime();

    #pragma omp parallel num_threads(numThreads)
    {
        int threadID = omp_get_thread_num();
        int localCount = 0;

        for (int i = start + threadID; i <= end; i += numThreads) {
            if (isPrime(i)) {
                localCount++;
            }
        }

        primesPerThread[threadID] = localCount;

        #pragma omp atomic
        totalPrimes += localCount;
    }

    double endTime = omp_get_wtime();
    std::cout << "Striping Partitioning:\n";
    for (int i = 0; i < numThreads; i++) {
        std::cout << "Thread " << i << " found " << primesPerThread[i] << " primes\n";
    }
    std::cout << "Total primes found: " << totalPrimes << "\n";
    std::cout << "Total runtime: " << endTime - startTime << " seconds\n";

    return totalPrimes;
}

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
*/


int main() {
    
    // Ask user for a number
    int userInput;
    std::cout << "Enter a number: ";
    std::cin >> userInput;

    if (isPrime(userInput)) {
        std::cout << userInput << " is a prime number\n";
    } else {
        std::cout << userInput << " is not a prime number\n";
    }

    // Ask user for a range
    int userRange1, userRange2;
    std::cout << "Enter a range: ";
    std::cin >> userRange1 >> userRange2;
    //start timer
    auto start1 = std::chrono::high_resolution_clock::now();
    //test blocking
    std::cout << "There are " << countPrimesBlocking(userRange1, userRange2) << " prime numbers between " << userRange1 << " and " << userRange2 << "\n";
    // End the timer
    auto end1 = std::chrono::high_resolution_clock::now();
    //calculate time between
    std::chrono::duration<double> elapsed1 = end1 - start1;

    //STRIPING
    //start timer again
    auto start2 = std::chrono::high_resolution_clock::now();
    //test striping
    std::cout << "There are " << countPrimesStriping(userRange1, userRange2) << " prime numbers between " << userRange1 << " and " << userRange2 << "\n";
    // End the timer
    auto end2 = std::chrono::high_resolution_clock::now();
    //calculate time between
    std::chrono::duration<double> elapsed2 = end2 - start2;

    //print times
    std::cout << "Time taken to execute Blocking: " << elapsed1.count() << " seconds\n";
    std::cout << "Time taken to execute Striping: " << elapsed2.count() << " seconds\n";


    return 0;
}
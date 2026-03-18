#include <stdio.h>
#include <time.h>

int isPrime(int n) {
    //primes are greater than 1
    if (n <= 1) {
        return 0;
    }
    //primes are only divisible by themselves and 1. if it is divisible to a round number by any other number, it is not prime
    for (int i = 2; i < n; i++) {
        //mod division to check if n is divisible by i to a whole number
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

int countPrimes(int start, int end){
    int count = 0;
    for (int i = start; i <= end; i++) {
    //for each number between start and end, check if it is prime using the isPrime function. 
        if (isPrime(i)) {
            count++;
        }
    }
    return count;
}

void main() {
    //code to set timer at the CPU and start it
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    //ask user for a number and set that number to userInput
    int userInput;
    printf("Enter a number: ");
    scanf("%d", &userInput);

    //if the number is prime, print that it is prime, if not, print that it is not prime
    if (isPrime(userInput)) {
        printf("%d is a prime number\n", userInput);
    } else {
        printf("%d is not a prime number\n", userInput);
    }

    //ask user for two numbers, and set those numbers to userRange1 and userRange2
    int userRange1;
    int userRange2;
    printf("Enter a range: ");
    scanf("%d %d", &userRange1, &userRange2);
    
    //print the number of prime numbers between userRange1 and userRange2 using the countPrimes function
    printf("There are %d prime numbers between %d and %d\n", countPrimes(userRange1, userRange2), userRange1, userRange2);

    //code to stop the timer and print the time taken. Clocks per second is 1000. double is used to get a more accurate time because of decimals
    end  = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f\n", cpu_time_used);
}
// compile: g++ lab4server.cc DENSocket.cc DENServerSocket.cc -o lab4server
#include <stdio.h>
#include "DENSocket.h"
#include "DENServerSocket.h"

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
    int port = 7000;
    DENServerSocket* ss = new DENServerSocket(port);
    printf("Server waiting for clients\n");
    DENSocket* s = ss->acceptConnection();
    printf("Client connected\n");
    int x = s->readInt();
    //do i add the count primes in a range here?
    printf("Server read %d\n", x);
    
    int y = x*2;
    s->writeInt(y);
    s->closeSocket();
    return 0;
}
// compile: g++ lab4client.cc DENSocket.cc DENServerSocket.cc -o lab4client
#include <stdio.h>
#include "DENSocket.h"



int main() {


    int port = 7000;
    DENSocket* s = new DENSocket("localhost", port);
    int x = 5;
    s->writeInt(x);
    int y = s->readInt();
    printf("Returned: %d\n", y);
    s->closeSocket();
    return 0;
}
#ifndef DENServerSocket_h
#define DENServerSocket_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "DENSocket.h"

class DENServerSocket {
    private:
        int serverSocket;

    public:
    
        DENServerSocket(int portno);
        DENSocket* acceptConnection();
        void closeSocket();
};

#endif
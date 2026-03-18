#ifndef DENSocket_h
#define DENSocket_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

class DENSocket {
    private:
        int mySocket;

    public:

        DENSocket(std::string ip, int portno);  // the user calls this directly to make a call
        DENSocket(int portno);  // this is called by the serversocket upon accept
        void writeInt(int x);
        int readInt();
        void closeSocket();
};

#endif
#include "DENSocket.h"

DENSocket::DENSocket(int socket) {
    this->mySocket = socket;
}

DENSocket::DENSocket(std::string host, int portno) {

  // Convert the c++ string to char*
  char* hostCstring = const_cast<char*>(host.c_str());
 
  // Socket pointer
  int sockfd;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    fprintf(stderr,"ERROR opening socket\n");
    exit(0);
  }

  // server address structure
  struct sockaddr_in serv_addr;

  // Set all the values in the server address to 0
  memset(&serv_addr, '0', sizeof(serv_addr)); 

  // Setup the type of socket (internet vs filesystem)
  serv_addr.sin_family = AF_INET;

   // Setup the port number
  // htons - is host to network byte order
  // network byte order is most sig byte first
  //   which might be host or might not be
  serv_addr.sin_port = htons(portno);


  // Setup the server host address
  struct hostent *server;
  server = gethostbyname(hostCstring);
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }
  memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);  /// dest, src, size

  // Connect to the server
  if (connect(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    printf("ERROR connecting\n");
    exit(0);
  }

  this->mySocket = sockfd;
}

void DENSocket::writeInt(int x) {

  int n = write(this->mySocket, &x, sizeof(int));
  if (n < 0) {
    printf("ERROR writing to socket\n");
    exit(0);
  }
}

int DENSocket::readInt() {
  int x;

  int n = read(this->mySocket, &x, sizeof(int));
  if (n < 0) {
    printf("ERROR reading from socket\n");
    exit(0);
  }

  return x;
}

void DENSocket::closeSocket() {
    close(this->mySocket);
}
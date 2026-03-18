#include "DENServerSocket.h"


DENServerSocket::DENServerSocket(int portno) {

  // Get a socket of the right type
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    printf("ERROR opening socket");
    exit(1);
  }

  // server address structure
  struct sockaddr_in serv_addr;

  // Set all the values in the server address to 0
  memset(&serv_addr, '0', sizeof(serv_addr)); 

  // Setup the type of socket (internet vs filesystem)
  serv_addr.sin_family = AF_INET;

  // Basically the machine we are on...
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  // Setup the port number
  // htons - is host to network byte order
  // network byte order is most sig bype first
  //   which might be host or might not be
  serv_addr.sin_port = htons(portno);

  // Bind the socket to the given port
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    printf("ERROR on binding\n");
    exit(1);
  }

  // set it up to listen
  listen(sockfd,5);

  this->serverSocket = sockfd;
}


DENSocket* DENServerSocket::acceptConnection() {

  int newsockfd;
  struct sockaddr_in cli_addr;
  socklen_t clilen = sizeof(cli_addr);

  // Wait for a call
  newsockfd = accept(this->serverSocket, (struct sockaddr *) &cli_addr, &clilen);
  if (newsockfd < 0) {
    printf("ERROR on accept");
    exit(1);
  }

  return new DENSocket(newsockfd);
}


void DENServerSocket::closeSocket() {
    close(this->serverSocket);
}
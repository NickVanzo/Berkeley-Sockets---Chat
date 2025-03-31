#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include "socketutil.h"
#include <pthread.h>
#include <unistd.h>

struct AcceptedSocket
{
    int acceptedSocketFD;
    struct sockaddr_in address;
    int error;
    bool acceptedSuccessfully;
};

struct AcceptedSocket acceptedSockets[10];
int acceptedSocketsCount = 0;

std::shared_ptr<AcceptedSocket> acceptIncomingConnection(int socketFD);
void* receiveIncomingData(void* socketFD);
void startAcceptingIncomingConnections(int);
void* receiveIncomingDataOnSeparateThread(struct AcceptedSocket* clientSocket);
void broadcastMessage(char *msg, int senderSocketFD);

void startAcceptingIncomingConnections(int serverSocketFD) {
    while(true)
    {
        auto clientSocket = acceptIncomingConnection(serverSocketFD);
        acceptedSockets[acceptedSocketsCount++] = *clientSocket;

        receiveIncomingDataOnSeparateThread(clientSocket.get());
    }
}


void* receiveIncomingDataOnSeparateThread(struct AcceptedSocket* clientSocket)
{
    std::cout << "Success: " << clientSocket->acceptedSuccessfully << std::endl;
    std::cout << "FD: " << clientSocket->acceptedSocketFD << std::endl;
    pthread_t pid;
    pthread_create(&pid, NULL, receiveIncomingData, reinterpret_cast<void*>(clientSocket->acceptedSocketFD));
}

void* receiveIncomingData(void* socketFD) {
    char buffer[1024];
    int sockfd = reinterpret_cast<intptr_t>(socketFD);

    while(true)
    {
        size_t amountReceived = recv(sockfd, buffer, 1024, 0);
        if(amountReceived > 0)
        {
            buffer[amountReceived] = 0;
            std::cout << "Received message: " << buffer << std::endl;

            broadcastMessage(buffer, sockfd);
        }

        if(amountReceived == 0)
            break;
    }

    close(sockfd);
}

void broadcastMessage(char *msg, int senderSocketFD) {
    for(int i = 0; i < acceptedSocketsCount; i++)
    {
        if(acceptedSockets[i].acceptedSocketFD != senderSocketFD)
        {
            send(
                    acceptedSockets[i].acceptedSocketFD,
                    msg,
                    strlen(msg),
                    0
                    );
        }
    }
}

std::shared_ptr<AcceptedSocket> acceptIncomingConnection(int socketFD) {
    struct sockaddr_in clientAddress;
    socklen_t clientAddressSize = sizeof(struct sockaddr_in);
    int clientSocketFD = accept(socketFD, (struct sockaddr*)&clientAddress, &clientAddressSize);

    std::shared_ptr<AcceptedSocket> acceptedSocket = std::make_shared<AcceptedSocket>();

    acceptedSocket->address = clientAddress;
    acceptedSocket->acceptedSocketFD = clientSocketFD;
    acceptedSocket->acceptedSuccessfully = clientSocketFD > 0;

    if(!acceptedSocket->acceptedSuccessfully)
        acceptedSocket->error = clientSocketFD;

    return acceptedSocket;
}

/**
 * After creating the socket, listening and binding we start accepting incoming connections by calling
 * `startAcceptingIncomingConnections`. `startAcceptingIncomingConnections` creates a separate thread.
 */
int main(int argc, char* argv[])
{
    std::shared_ptr<SocketUtil> utils = std::make_shared<SocketUtil>();
    int socketFD = utils->createTCPIPv4Socket();
    std::shared_ptr<struct sockaddr_in> serverAddress = utils->createIPv4Address("", 8081);


    int res = bind(socketFD,  (struct sockaddr*) serverAddress.get(), sizeof(serverAddress));

    if(res == 0)
        std::cout << "Socket was bound successfully" << std::endl;
    else
        std::cout << "Socket was not bound " << strerror(errno) << std::endl;

    //Queue up to 10 connections
    int listenResult = listen(socketFD, 10);

    startAcceptingIncomingConnections(socketFD);

    shutdown(socketFD, SHUT_RDWR);

    return 0;
}


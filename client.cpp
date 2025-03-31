#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <memory>
#include <unistd.h>
#include "socketutil.h"
#include <string>
#include <sstream>
void listenForMessagesOnNewThreadWrapper(void* socketFD);

void* listenForMessages(void *socketFD);

int main(int argc, char* argv[])
{
    std::shared_ptr<SocketUtil> utils = std::make_shared<SocketUtil>();

    int socketFD = utils->createTCPIPv4Socket();
    std::shared_ptr<struct sockaddr_in> address = utils->createIPv4Address("127.0.0.1", 8081);
    int result = connect(socketFD, (struct sockaddr*) address.get(), sizeof(address));

    if(result == 0)
        std::cout << "Connection successful" << std::endl;
    else
        std::cout << "Connection failed" << std::endl;

    std::string name;
    std::cout << "Enter your username: ";
    std::cin >> name;

    listenForMessagesOnNewThreadWrapper(reinterpret_cast<void*>(socketFD));

    std::cout << "Type..." << std::endl;

    while(true)
    {
        std::string line;
        getline(std::cin, line);
        std::stringstream ss;

        ss << name << ":" << line;
        std::string formattedString = ss.str();
        std::cout << formattedString << std::endl;

        if(!line.empty())
        {
            if(line == "exit")
                break;
            size_t amountWasSent = send(socketFD, formattedString.c_str(), formattedString.size(), 0);
        }
    }

    close(socketFD);

    return 0;
}

void listenForMessagesOnNewThreadWrapper(void* socketFD) {
    
    pthread_t pid;
    pthread_create(&pid, NULL, listenForMessages, socketFD);
}

void* listenForMessages(void *socketFD) {
    char buffer[1024];

    int sockfd = reinterpret_cast<intptr_t>(socketFD);

    while(true)
    {
        size_t amountReceived = recv(sockfd, buffer, 1024, 0);
        if(amountReceived > 0)
        {
            buffer[amountReceived] = 0;
            std::cout << buffer << std::endl;
        }

        if(amountReceived == 0)
            break;
    }

    close(sockfd);
}


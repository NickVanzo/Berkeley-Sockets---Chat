//
// Created by Nicolò Vanzo on 29/03/25.
//

#include "socketutil.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory>
#include <iostream>

std::shared_ptr<struct sockaddr_in> SocketUtil::createIPv4Address(const char *ip, int port) {
    auto address = std::make_shared<struct sockaddr_in>();
    address->sin_family = AF_INET;
    address->sin_port = htons(port);

    if(strlen(ip) == 0) {
        address->sin_addr.s_addr = INADDR_ANY;
        std::cout << "Creating address for localhost" << std::endl;
    }
    else
        inet_pton(AF_INET, ip, &address->sin_addr.s_addr);

    return address;
}

int SocketUtil::createTCPIPv4Socket() {
    return socket(AF_INET, SOCK_STREAM, 0);
}

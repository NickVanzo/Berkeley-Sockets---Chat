//
// Created by Nicolò Vanzo on 29/03/25.
//

#pragma once
#include <memory>

class SocketUtil {
public:
    int createTCPIPv4Socket();
    std::shared_ptr<struct sockaddr_in> createIPv4Address(const char *ip, int port);
};
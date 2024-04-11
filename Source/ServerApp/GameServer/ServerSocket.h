#pragma once
#include <winsock2.h>
#include <iostream>
#include <vector>

const int   PORTNUM = 12345,
            MAXCONNECTIONS = 20;

class ServerSocket {
    private:
        SOCKET sock;
        std::vector< SOCKET > clientList;
        
    public:
        ServerSocket();
        bool isFull();
        void socketListen(int maxClients);
        SOCKET acceptConnection();
        int receiveData(SOCKET client, char *buf);
        void sendData(SOCKET client, char *buf, int dataSize);
        ~ServerSocket();
};
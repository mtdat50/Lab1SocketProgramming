#pragma once
#include <winsock2.h>
#include <iostream>
#include <vector>

const char SERVERIP[] = "127.0.0.1";
const int   PORTNUM = 12345;

class ClientSocket {
    private:
        SOCKET sock;
    public:
        ClientSocket();
        void connectToServer();
        int receiveData(char *buf);
        void sendData(char *buf, int dataSize);
        ~ClientSocket();
};
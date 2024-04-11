#include "ClientSocket.h"



ClientSocket::ClientSocket() {
    // try creating a socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        // Initialize Winsock
        if (WSAStartup(MAKEWORD(2, 2), nullptr) != 0) { 
            std::cout << "ClientSocket: Failed to initialize Winsock.\n";
            return;
        }
        else
            sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }

    // Set the socket to non-blocking mode
    u_long mode = 1;
    int iResult = ioctlsocket(sock, FIONBIO, &mode);
    if (iResult != NO_ERROR) {
        printf("ClientSocket: ioctlsocket failed: %ld\n", iResult);
        return;
    }
}

ClientSocket::~ClientSocket() {
    closesocket(sock);
    WSACleanup();
}

void ClientSocket::connectToServer() {
    // Set up the server address structure
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVERIP);
    serverAddr.sin_port = htons(PORTNUM);

    // Connect to the server
    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
        printf("Failed to connect to server.\n");
}

int ClientSocket::receiveData(char *buf) {
    return recv(sock, buf, sizeof buf, 0);
}

void ClientSocket::sendData(char *buf, int dataSize) {
    send(sock, buf, dataSize, 0);
}
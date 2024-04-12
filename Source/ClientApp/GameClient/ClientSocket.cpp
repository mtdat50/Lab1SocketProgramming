#include "../include/ClientSocket.h"



ClientSocket::ClientSocket() {
    // try creating a socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        // Initialize Winsock
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { 
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

bool ClientSocket::connectToServer() {
    // Set up the server address structure
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVERIP);
    serverAddr.sin_port = htons(PORTNUM);

    // Connect to the server
    connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    // Check if success
    fd_set writefds;
    FD_ZERO(&writefds);
    FD_SET(sock, &writefds);
    struct timeval tv;
    tv.tv_sec = 1;  // Timeout after 1 seconds
    tv.tv_usec = 0;
    int result = select(0, NULL, &writefds, NULL, &tv);
    if (result > 0 && FD_ISSET(sock, &writefds)) {
        int error;
        int len = sizeof(error);
        getsockopt(sock, SOL_SOCKET, SO_ERROR, (char*)&error, &len);
        if (error == 0) {
            std::cout << "ClientSocket: Connected to server successfully.\n";
            return true;
        }
    }

    return false;
}

int ClientSocket::receiveData(char *buf) {
    int dataSize = recv(sock, buf, 1, 0);
    if (dataSize <= 0) return dataSize;

    while (buf[dataSize - 1] != '\n')
        dataSize += recv(sock, &buf[dataSize], 1, 0);

    // std::cout << "receiveData: dataSize " << dataSize << '\n';
    return dataSize;
}

void ClientSocket::sendData(char *buf, int dataSize) {
    buf[dataSize++] = '\n';
    send(sock, buf, dataSize, 0);
}
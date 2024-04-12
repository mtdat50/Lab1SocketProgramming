#include "../include/ServerSocket.h"

ServerSocket::ServerSocket() {
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        // Initialize Winsock
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { 
            std::cout << "ServerSocket: Failed to initialize Winsock.\n";
            return;
        }
        else
            sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }
    
    // Set the socket to non-blocking mode
    u_long mode = 1;
    int iResult = ioctlsocket(sock, FIONBIO, &mode);
    if (iResult != NO_ERROR) {
        printf("ServerSocket: ioctlsocket failed: %ld\n", iResult);   
        return;
    }

    // Set up the server address structure
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORTNUM);

    // Bind the socket
    if (bind(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
        std::cout << "ServerSocket: Bind failed.\n";
}

ServerSocket::~ServerSocket() {
    closesocket(sock);
    for (SOCKET clientSoc: clientList)
        closesocket(clientSoc);
    WSACleanup();
}

void ServerSocket::socketListen(int maxClients) {
    // Listen for incoming connections
    listen(sock, maxClients);

    std::cout << "ServerSocket: Waiting for incoming connections...\n";
}

SOCKET ServerSocket::acceptConnection() {
    // Accept a connection from a client
    SOCKET clientSocket = accept(sock, nullptr, nullptr);
    if (clientSocket != INVALID_SOCKET)
        std::cout << "ServerSocket: Connection accepted.\n";

    return clientSocket;
}

bool ServerSocket::isFull() {
    return clientList.size() == MAXCONNECTIONS;
}


int ServerSocket::receiveData(SOCKET client, char *buf) {
    int dataSize = recv(client, buf, 1, 0);
    if (dataSize <= 0) return dataSize;

    while (buf[dataSize - 1] != '\n')
        dataSize += recv(client, &buf[dataSize], 1, 0);

    return --dataSize;
}


void ServerSocket::sendData(SOCKET client, char *buf, int dataSize) {
    buf[dataSize++] = '\n';
    send(client, buf, dataSize, 0);
}
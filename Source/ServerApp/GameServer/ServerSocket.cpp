#include "ServerSocket.h"

ServerSocket::ServerSocket() {
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        // Initialize Winsock
        if (WSAStartup(MAKEWORD(2, 2), nullptr) != 0) { 
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
    if (clientSocket == INVALID_SOCKET)
        std::cout << "ServerSocket: Failed to accept connection.\n";
    else {
        clientList.push_back(clientSocket);
        std::cout << "ServerSocket: Connection accepted.\n";
    }

    return clientSocket;
}

bool ServerSocket::isFull() {
    return clientList.size() == MAXCONNECTIONS;
}


int ServerSocket::receiveData(SOCKET client, char *buf) {
    return recv(client, buf, sizeof buf, 0);
}


void ServerSocket::sendData(SOCKET client, char *buf, int dataSize) {
    send(client, buf, dataSize, 0);
}
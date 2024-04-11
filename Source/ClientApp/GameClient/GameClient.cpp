#include "GameClient.h"

GameClient::GameClient() {
    clientSock.connectToServer();
}
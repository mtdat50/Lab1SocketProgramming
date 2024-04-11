#include <winsock2.h>
#include <iostream>
#include "GameServer/GameServer.h"
#include "GameServer/Data.h"

#pragma comment(lib, "ws2_32.lib")


int main() {
    int maxClients = 10;    
    int playersPerGame = 5;
    GameServer myGame(maxClients, playersPerGame);
    


    myGame.waitForPlayers();
    
    int trackLength;
    std::cout << "Enter track length: ";
    std::cin >> trackLength;
    
    myGame.start(trackLength);
    while (!myGame.checkForWinner()) {
        myGame.startQuestion();
        myGame.receiveAndProcessAnswers();
    }
    

    return 0;
}

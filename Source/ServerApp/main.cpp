#include <winsock2.h>
#include <iostream>
#include "include/GameServer.h"
#include "include/Data.h"

#pragma comment(lib, "ws2_32.lib")


int main() {
    int playersPerGame = 1;
    int maxClients = 10;    
    GameServer myGame(playersPerGame, maxClients);
    


    myGame.waitForPlayers();
    
    int trackLength;
    std::cout << "Enter track length: ";
    std::cin >> trackLength;
    // trackLength = 5;
    
    myGame.start(trackLength);
    while (!myGame.checkForWinner()) {
        myGame.startQuestion();
        myGame.receiveAndProcessAnswers();
    }
    

    return 0;
}

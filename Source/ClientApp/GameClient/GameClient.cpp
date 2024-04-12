#include "../include/GameClient.h"
#include "../include/ClientSocket.h"
#include <iostream>

void player::init(int _playerID, std::string _name) {
    playerID = _playerID;
    name = _name;
    std::cout << "name: " << name << '\n';
}




ClientSocket clientSock;
GameClient::GameClient() {
    while (!clientSock.connectToServer());
}

// return -1 unable to send to server or registration rejected (server is not listening for registration),
//         0 name duplicated,
//         1 success.
int GameClient::playerRegistration(std::string playerName) {
    ACTION action;
    action.actionType = PLAYER_REGISTRATION;
    action.playerName = playerName;

    char buf[128];
    int dataSize = action2Data(action, buf);
    clientSock.sendData(buf, dataSize);

    // result from server
    dataSize = clientSock.receiveData(buf);
    if (dataSize == 0)
        return -1;
    
    action = data2Action(buf, dataSize);
    if (action.actionType == GAMESERVER_REJECT_REGISTRATION)
        return -1;
    
    if (action.actionType == GAMESERVER_PLAYERNAME_DUPLICATED)
        return 0;
    
    // register successfully
    myPlayer.init(action.playerID, playerName);
    return 1;
}

// return <nPlayers, maxPlayers> 
// if starting game, return <-1, -1>
std::pair<int, int> GameClient::waitForServerStarting() {
    char buf[128];
    int dataSize = clientSock.receiveData(buf);
    ACTION action = data2Action(buf, dataSize);
    
    if (action.actionType == GAMESERVER_ANNOUNCE_STARTING) {
        std::cout << "waitForServerStarting: GAMESERVER_ANNOUNCE_STARTING\n";
        std::cout << "nplayers: " << action.nameList.size() << '\n';
        int i = 0;
        for (std::string name: action.nameList) {
            std::cout << "player name: " << name << '\n';
            player p;
            p.init(i, name);
            playerList.push_back(p);
            ++i;
        }

        return std::pair(-1, -1);
    }

    if (action.actionType == GAMESERVER_PLAYERS_COUNT) {
        std::cout << "waitForServerStarting: GAMESERVER_PLAYERS_COUNT\n";
        nPlayerWaiting = action.nPlayers,
        maxPlayers = action.playersPerGame;
    }
    
    return std::pair(nPlayerWaiting, maxPlayers);// do nothing
}


bool GameClient::question() {
    char buf[128];
    int dataSize = clientSock.receiveData(buf);
    if (dataSize <= 0) return false;
    ACTION action = data2Action(buf, dataSize);
    // std::cout << "GameClient::question: actiontype: " << action.actionType << '\n';

    if (action.actionType != GAMESERVER_QUESTION) 
        return false;
    questionID = action.questionID;
    char op[] = "+-*/";
    std::cout << "Question: " << action.operand1 << ' ' << op[action._operator] << ' ' << action.operand2 << '\n';
    return true;
}

void GameClient::playerAnswer() {
    float answer;
    std::cout << "Player's answer: ";
    std::cin >> answer;

    char buf[128];
    ACTION action;
    action.questionID = questionID;
    action.actionType = PLAYER_ANSWER;
    action.answer = answer;
    int dataSize = action2Data(action, buf);
    clientSock.sendData(buf, dataSize);
}
#include "GameServer.h"



player::player(int _playerID, std::string _name, SOCKET _sock) {
    playerID = _playerID;
    name = _name;
    sock = _sock;
}

int player::getID() {
    return playerID;
}

std::string player::getName() {
    return name;
}

SOCKET player::getSocket() {
    return sock;
}

int player::getPoints() {
    return points;
}

void player::updatePoints(int change) {
    points += change;
}

int player::updateIncorrectStreak(bool correct) {
    if (correct)
        incorrectStreak = 0;
    else
        ++incorrectStreak;
    return incorrectStreak;
}

void player::disqualify() {
    disqualified = true;
}

bool player::isDisqualified() {
    return disqualified;
}



GameServer::GameServer(int _playersPerGame, int maxClients) {
    playersPerGame = _playersPerGame;
    serverSock.socketListen(maxClients);
}

bool GameServer::isFull() {
    return playerList.size() == playersPerGame;
}

bool GameServer::playerRegistration(std::string playerName, SOCKET playerSock) {
    // check name duplication
    for (player p: playerList)
        if (p.getName() == playerName)
            return false;
    
    playerList.push_back(player(playerList.size(), playerName, playerSock));
    return true;
}

void GameServer::waitForPlayers() {
    std::vector< SOCKET > clientList;
    while (isFull()) {
        // check for connection request
        SOCKET clientSoc = serverSock.acceptConnection();
        clientList.push_back(clientSoc);
        
        // check for player registration from clients
        for (SOCKET client: clientList) {
            char buf[64];
            int dataSize = serverSock.receiveData(client, buf);
            if (dataSize <= 0) 
                continue;
            
            ACTION action = data2Action(buf, dataSize);
            if (action.actionType != PLAYER_REGISTRATION)
                continue;

            // server reply to client
            if (playerRegistration(action.playerName, client)) 
                action.actionType = GAMESERVER_ACCEPT_REGISTRATION;
            else // playername is used
                action.actionType = GAMESERVER_PLAYERNAME_DUPLICATED;
            dataSize = action2Data(action, buf);
            serverSock.sendData(client, buf, dataSize);

            if (isFull()) break;
        }
    }

    std::cout << "All players have registered.\n";
}

void GameServer::start(int _trackLength) {
    trackLength = _trackLength;

    // announce to players
    char buf[64];
    ACTION action;
    action.actionType = GAMESERVER_ANNOUNCE_STARTING;
    action.trackLength = trackLength;
    int dataSize = action2Data(action, buf);

    for (player p: playerList)
        serverSock.sendData(p.getSocket(), buf, dataSize);
}

bool GameServer::checkForWinner() {
    for (player p: playerList)
        if (p.getPoints() == trackLength)
            return true;

    return false;
}

int generateRandomInt(int l, int r) {
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(l, r);

    return distrib(gen);
}

void GameServer::startQuestion() {
    // generate question
    ACTION action;
    action.actionType = GAMESERVER_QUESTION;
    action.questionID = currentQuestionID = time(nullptr);
    action.operand1 = generateRandomInt(-10000, 10000);
    action.operand2 = generateRandomInt(-10000, 10000);
    action._operator = generateRandomInt(0, 3);
    
    // send question
    char buf[64];
    int dataSize = action2Data(action, buf);
    for (player p: playerList)
        serverSock.sendData(p.getSocket(), buf, dataSize);


    switch (action._operator) {
        case 0:
            correctAnswer = action.operand1 + action.operand2;
            break;
        case 1:
            correctAnswer = action.operand1 - action.operand2;
            break;
        case 2:
            correctAnswer = action.operand1 * action.operand2;
            break;
        case 3:
            correctAnswer = action.operand1 / action.operand2;
            break;
    }
}


const u_int64 TIMELIMIT = 15;
void GameServer::receiveAndProcessAnswers() {
    // clock
    auto started = std::chrono::high_resolution_clock::now();
    auto done = std::chrono::high_resolution_clock::now();
    u_int64 timeCounting = std::chrono::duration_cast<std::chrono::seconds>(done-started).count();

    // receive answers for TIMELIMIT seconds
    bool isCorrect[10];
    memset(isCorrect, 0, sizeof(isCorrect));
    int wrongAnswerCnt = 0;
    int firstCorrect = -1;
    while (timeCounting < TIMELIMIT) {
        for (player p: playerList) {
            if (p.isDisqualified()) continue;

            char buf[64];
            int dataSize = serverSock.receiveData(p.getSocket(), buf);
            if (dataSize <= 0) 
                continue;
            
            ACTION action = data2Action(buf, dataSize);
            if (action.actionType != PLAYER_ANSWER || action.questionID != currentQuestionID)
                continue;

            // check the answer
            if (std::abs(correctAnswer - action.answer) < 1e6) {
                isCorrect[p.getID()] = true;
                if (firstCorrect == -1)
                    firstCorrect = p.getID();
            }
            else
                ++wrongAnswerCnt;
        }

        done = std::chrono::high_resolution_clock::now();
        timeCounting = std::chrono::duration_cast<std::chrono::seconds>(done-started).count();
    }

    // update points
    for (player p: playerList) {
        if (p.isDisqualified()) continue;
        p.updateIncorrectStreak(isCorrect[p.getID()]);

        if (isCorrect[p.getID()]) {
            p.updatePoints(1);
            if (firstCorrect == p.getID())
                p.updatePoints(wrongAnswerCnt);
        }
        else
            p.updatePoints(-1);

        // check if 3 consecutively incorrect
        if (p.updateIncorrectStreak(isCorrect[p.getID()]) == 3) 
            p.disqualify();
    }


    // announce result to players
    char buf[64];
    ACTION action;
    action.actionType = GAMESERVER_ANNOUNCE_RESULT;
    action.correctAnswer = correctAnswer;
    for (player p: playerList)
        if (p.isDisqualified())
            action.points.push_back(-p.getPoints());
        else
            action.points.push_back(p.getPoints());
    
    int dataSize = action2Data(action, buf);
    for (player p: playerList)
        serverSock.sendData(p.getSocket(), buf, dataSize);
}
#pragma once
#include <string>
#include <vector>
#include <winsock2.h>
#include <time.h>
#include <random>
#include <chrono>
#include <thread>
#include <math.h>

#include "ServerSocket.h"
#include "Data.h"


class player {
    private:
        int playerID;
        std::string name;
        SOCKET sock;
        int points;
        int incorrectStreak;
        bool disqualified;
    
    public:
        player(int _playerID, std::string _name, SOCKET _sock);
        int getID();
        std::string getName();
        SOCKET getSocket();
        int getPoints();
        void updatePoints(int change);
        int updateIncorrectStreak(bool correct);
        void disqualify();
        bool isDisqualified();
};



class GameServer {
    private:
        int playersPerGame;
        ServerSocket serverSock;
        int trackLength;
        uint64_t currentQuestionID;
        float correctAnswer;
        std::vector< player > playerList;
    
    public:
        GameServer(int _nPlayers, int maxClients);
        bool isFull();
        bool playerRegistration(std::string playerName, SOCKET playerSock);
        void waitForPlayers();
        void start(int _trackLength);
        bool checkForWinner();
        void startQuestion();
        void receiveAndProcessAnswers();
};

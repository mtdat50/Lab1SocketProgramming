#pragma once
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include "../include/Data.h"

class player {
    private:
        int playerID;
        std::string name;
        int points;
        int incorrectStreak;
        bool disqualified;
    
    public:
        void init(int _playerID, std::string _name);
        int getID();
        std::string getName();
        int getPoints();
        void updatePoints(int change);
        int updateIncorrectStreak(bool correct);
        void disqualify();
        bool isDisqualified();
};


class GameClient {
    private:
        player myPlayer;
        int nPlayerWaiting, maxPlayers;
        std::vector< player > playerList;
        uint64_t questionID;


    public:
        GameClient();
        int playerRegistration(std::string playerName);
        std::pair< int, int> waitForServerStarting();
        bool question();
        void playerAnswer();
};

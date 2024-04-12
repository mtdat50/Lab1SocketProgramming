#pragma once
#include <string>
#include <vector>
#include <string.h>


enum ACTIONTYPE {
    ERROR_ACTION,
    PLAYER_REGISTRATION,
    PLAYER_ANSWER,
    GAMESERVER_ACCEPT_REGISTRATION,
    GAMESERVER_REJECT_REGISTRATION,
    GAMESERVER_PLAYERNAME_DUPLICATED,
    GAMESERVER_PLAYERS_COUNT,
    GAMESERVER_ANNOUNCE_STARTING,
    GAMESERVER_QUESTION,
    GAMESERVER_ANNOUNCE_RESULT,
};

struct ACTION {
    ACTIONTYPE actionType;
    int playersPerGame, nPlayers;

    int trackLength;
    std::vector< std::string > nameList;

    std::string playerName;
    int playerID;

    uint64_t questionID;
    int operand1, operand2, _operator; // + - * /

    float answer, correctAnswer;

    std::vector< int > points;
};

ACTION data2Action(char *buf, int dataSize);
int action2Data(ACTION action, char *buf);
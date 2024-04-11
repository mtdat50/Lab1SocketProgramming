#pragma once
#include <string>
#include <vector>


enum ACTIONTYPE {
    PLAYER_REGISTRATION,
    PLAYER_ANSWER,
    GAMESERVER_ACCEPT_REGISTRATION,
    GAMESERVER_REJECT_REGISTRATION,
    GAMESERVER_PLAYERNAME_DUPLICATED,
    GAMESERVER_ANNOUNCE_STARTING,
    GAMESERVER_QUESTION,
    GAMESERVER_ANNOUNCE_RESULT,
};

struct ACTION {
    ACTIONTYPE actionType;
    int trackLength;
    std::string playerName;

    uint64_t questionID;
    int operand1, operand2, _operator; // + - * /

    float answer;
    float correctAnswer;

    std::vector< int > points;
};

ACTION data2Action(char *buf, int dataSize);
int action2Data(ACTION action, char *buf);
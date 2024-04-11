#include "Data.h"


// take data received from client, parse into ACTION
ACTION data2Action(char *buf, int dataSize) {
    ACTION res;

    switch (buf[0]) {
        case PLAYER_REGISTRATION:
            res.actionType = PLAYER_REGISTRATION;
            buf[dataSize] = '\0';
            res.playerName = &buf[1];
            break;
        
        case PLAYER_ANSWER:
            res.actionType = PLAYER_ANSWER;
            res.answer = *((float*)&buf[1]);

        default:
            break;
    }

    return res;
}

// take an ACTION, generate data to send to client
int action2Data(ACTION action, char *buf) {
    int dataSize;

    switch (action.actionType) {
        case GAMESERVER_ACCEPT_REGISTRATION:
            buf[0] = GAMESERVER_ACCEPT_REGISTRATION;
            dataSize = 1;
            break;
        
        case GAMESERVER_PLAYERNAME_DUPLICATED:
            buf[0] = GAMESERVER_PLAYERNAME_DUPLICATED;
            dataSize = 1;
        
        case GAMESERVER_ANNOUNCE_STARTING:
            buf[0] = GAMESERVER_ANNOUNCE_STARTING;
            buf[1] = action.trackLength;
            dataSize = 2;

        case GAMESERVER_QUESTION:
            buf[0] = GAMESERVER_QUESTION;
            *((uint64_t *)&buf[1]) = action.questionID;
            *((int *)&buf[9]) = action.operand1;
            *((int *)&buf[13]) = action.operand2;
            buf[14] = action._operator;
            dataSize = 15;

        case GAMESERVER_ANNOUNCE_RESULT:
            buf[0] = GAMESERVER_ANNOUNCE_RESULT;
            *((float *)&buf[1]) = action.correctAnswer;
            int i = 5;
            for (int point: action.points) {
                *((int *)&buf[i]) = point;
                i += 4;
            }
            dataSize = i;

        default:
            break;
    }

    return dataSize;
}
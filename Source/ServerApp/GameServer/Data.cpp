#include "../include/Data.h"
#include <iostream>

// take data received from client, parse into ACTION
ACTION data2Action(char *buf, int dataSize) {
    ACTION action;

    action.actionType = (ACTIONTYPE) buf[0];
    switch (buf[0]) {
        case PLAYER_REGISTRATION:
            buf[dataSize] = '\0';
            action.playerName = &buf[1];
            break;
        
        case PLAYER_ANSWER:
            action.questionID = *((uint64_t*)&buf[1]);
            std::memcpy(&action.answer, &buf[9], sizeof(double));
            std::cout << "data2Action PLAYER_ANSWER: " << action.answer << '\n';
            break;

        default:
            action.actionType = ERROR_ACTION;
            break;
    }

    return action;
}

// take an ACTION, generate data to send to client
int action2Data(ACTION action, char *buf) {
    int dataSize;

    memset(buf, 0, sizeof(buf));
    buf[0] = action.actionType;
    switch (action.actionType) {
        case GAMESERVER_ACCEPT_REGISTRATION:
            buf[1] = action.playerID;
            dataSize = 2;
            break;
        
        case GAMESERVER_PLAYERNAME_DUPLICATED:
            dataSize = 1;
            break;

        case GAMESERVER_PLAYERS_COUNT:
            buf[1] = action.nPlayers;
            buf[2] = action.playersPerGame;
            dataSize = 3;
            break;
        
        case GAMESERVER_ANNOUNCE_STARTING:
            buf[1] = action.trackLength;
            buf[2] = action.nameList.size();
            dataSize = 3;
            for (std::string name: action.nameList) {
                std::strcpy(&buf[dataSize], name.c_str());
                dataSize += 10;
            }
            break;

        case GAMESERVER_QUESTION:{
            *((uint64_t*)&buf[1]) = action.questionID;
            *((uint32_t*)&buf[9]) = static_cast<uint32_t>(action.operand1);
            *((uint32_t*)&buf[13]) = static_cast<uint32_t>(action.operand2);
            //     buf[i + 9] = static_cast<uint8_t>((action.operand1 >> (8 * i)) & 0xFF);
            // for (int i = 0; i < 4; ++i)
            //     buf[i + 13] = static_cast<uint8_t>((action.operand2 >> (8 * i)) & 0xFF);
            buf[17] = action._operator;
            dataSize = 18;
            // std::cout << "GAMESERVER_QUESTION " << *((uint64_t *)&buf[1]) << ' ' << *((int *)&buf[9]) << ' ' << *((int *)&buf[13]) << '\n';
            break;

        }

        case GAMESERVER_ANNOUNCE_RESULT:
            *((float *)&buf[1]) = action.correctAnswer;
            dataSize = 5;
            for (int point: action.points) {
                *((int *)&buf[dataSize]) = point;
                dataSize += 4;
            }
            break;

        default:
            break;
    }

    // std::cout << "Data: " << "dataSize: " << dataSize << ' ' << "actiontype: " << (ACTIONTYPE) buf[0] << '\n';
    return dataSize;
}
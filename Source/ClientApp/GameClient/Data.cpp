#include "../include/Data.h"
#include <iostream>

// take data received from server, parse into ACTION
ACTION data2Action(char *buf, int dataSize) {
    // std::cout << "Data: " << "dataSize: " << dataSize << ' ' << "actiontype: " << (ACTIONTYPE) buf[0] << '\n';
    ACTION action;
    int nPlayers, i, j;
    char name[11];

    action.actionType = (ACTIONTYPE) buf[0];
    switch (buf[0]) {
        case GAMESERVER_ACCEPT_REGISTRATION:
            action.playerID = buf[1];
            break;
        
        case GAMESERVER_REJECT_REGISTRATION:
            break;
        
        case GAMESERVER_PLAYERNAME_DUPLICATED:
            break;

        case GAMESERVER_PLAYERS_COUNT:
            action.nPlayers = buf[1];
            action.playersPerGame = buf[2];
            break;
        
        case GAMESERVER_ANNOUNCE_STARTING:
            action.trackLength = buf[1];
            action.nPlayers = buf[2];
            i = 3;
            while (nPlayers) {
                for (j = 0; j < 10; ++j, ++i)
                    name[j] = buf[i];
                name[j + 1] = '\0';
                action.nameList.push_back(name);
                --nPlayers;
            }
            break;

        case GAMESERVER_QUESTION: {
            std::cout << "Data: Question ";
            action.questionID = *((uint64_t*)&buf[1]);
            std::cout << action.questionID << ' ';

            action.operand1 = static_cast<int>(*((uint32_t*)&buf[9]));
            std::cout << action.operand1 << ' ';

            action.operand2 = static_cast<int>(*((uint32_t*)&buf[13]));
            std::cout << action.operand2 << ' ';

            action._operator =  buf[17];
            break;

        }

        default:
            action.actionType = ERROR_ACTION;
            break;
    }

    return action;
}

// take an ACTION, generate data to send to server
int action2Data(ACTION action, char *buf) {
    int dataSize;
    memset(buf, 0, sizeof(buf));

    buf[0] = action.actionType;
    switch (action.actionType) {
        case PLAYER_REGISTRATION:
            strcpy(&buf[1], action.playerName.c_str());
            dataSize = action.playerName.size() + 1;
            break;
        
        case PLAYER_ANSWER:
            *((uint64_t*) &buf[1]) = action.questionID;
            std::memcpy(&buf[9], &action.answer, sizeof(double));
            dataSize = 17;
            break;
            
        default:
            break;
    }

    return dataSize;
}
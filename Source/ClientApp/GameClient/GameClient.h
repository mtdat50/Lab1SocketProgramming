#include "ClientSocket.h"

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
        ClientSocket clientSock;
    public:
        GameClient();
};

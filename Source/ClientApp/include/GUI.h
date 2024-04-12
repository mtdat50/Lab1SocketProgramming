#pragma once
#include <string>
#include <cstring>
#include "raylib.h"
#include "GUIElements.h"

enum screen {
    REGISTRATION_SCREEN,
    WAITING_SCREEN,
    GAME_SCREEN
};

class GUI {
    private:
        int screenWidth, screenHeight;
        screen showing;

        InputBox playerNameInput;
        Button playButton;
        std::string regFailedMessage;


    public:
        GUI(int _screenWidth, int _screenHeight);
        screen getShowingScreen();
        void setShowingScreen(screen _showing);

        void ShowRegistrationScreen();
        std::string getPlayerNameInput();
        bool clickPlay();
        void updateByRegistrationResult(int regResult);

        void ShowWaitingScreen(int nPlayers, int maxPlayers);
};
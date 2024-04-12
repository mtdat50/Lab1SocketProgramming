// #include <winsock2.h>
#include <iostream>
#include "include/GameClient.h"
#include "include/raylib.h"
#include "include/GUI.h"

#pragma comment(lib, "ws2_32.lib")

#include <raylib.h>


int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Input Box Example (OOP)");
    SetTargetFPS(60);

    GUI myGUI(screenWidth, screenHeight);
    GameClient myGame;
    
    myGUI.setShowingScreen(REGISTRATION_SCREEN);
    while (!WindowShouldClose()) {
        switch (myGUI.getShowingScreen()) {
            case REGISTRATION_SCREEN:
                myGUI.ShowRegistrationScreen();
                if (myGUI.clickPlay()) {
                    int regResult = myGame.playerRegistration(myGUI.getPlayerNameInput());
                    myGUI.updateByRegistrationResult(regResult);
                    std::cout << "regResult: " << regResult << '\n';
                }
                break;
            
            case WAITING_SCREEN: {
                std::pair<int, int> playersCount = myGame.waitForServerStarting();

                if (playersCount.first > 0)
                    myGUI.ShowWaitingScreen(playersCount.first, playersCount.second);
                else if (playersCount.first == -1) {
                    myGUI.setShowingScreen(GAME_SCREEN);
                }
                break;
            }

            case GAME_SCREEN:
                // std::cout << "main: game screen\n";
                if (myGame.question())
                    myGame.playerAnswer();
                break;

            
            default:
                break;
        }
    }

    CloseWindow();

    return 0;
}

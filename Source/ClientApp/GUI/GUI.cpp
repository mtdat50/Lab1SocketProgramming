#include "../include/GUI.h"
#include <iostream>

GUI::GUI(int _screenWidth, int _screenHeight) {
    playerNameInput = InputBox("Enter your player name:", _screenWidth / 2 - 100, 180, 240, 50, 40);
    playButton = Button("PLAY", _screenWidth / 2 - 50, 260, 100, 50, 40);
}

screen GUI::getShowingScreen() {
    return showing;
}

void GUI::setShowingScreen(screen _showing) {
    showing = _showing;
}

void GUI::ShowRegistrationScreen() {
    BeginDrawing();
        ClearBackground(RAYWHITE);
        playerNameInput.Update();
        playButton.Update();

        playerNameInput.Draw();
        playButton.Draw();
        DrawText(regFailedMessage.c_str(), screenWidth / 2 - 100, 320, 20, RED);
    EndDrawing();
}

std::string GUI::getPlayerNameInput() {
    return playerNameInput.getText();
}

bool GUI::clickPlay() {
    return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && playButton.isHovered();
}

void GUI::updateByRegistrationResult(int regResult) {
    if (regResult == 1) {
        showing = WAITING_SCREEN;
        regFailedMessage = "";
    }
    else if (regResult == -1)
        regFailedMessage = "Unable to register. Please try again later.";
    else if (regResult == 0)
        regFailedMessage = "This name is taken. Please try another.";
}



void GUI::ShowWaitingScreen(int nPlayers, int maxPlayers) {
    int fontSize = 40;
    char text1[] = "Waiting for other players.";
    char text2[10];
    std::strcpy(text2, TextFormat("%d/%d", nPlayers, maxPlayers));
    
    BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText(text1, screenWidth / 2 - 50, screenHeight / 2 - 20, fontSize, RED);
        DrawText(text2, screenWidth / 2, screenHeight / 2, fontSize, RED);
    EndDrawing();
    // std::cout << "GUI::ShowWaitingScreen " << nPlayers << ' ' << maxPlayers << '\n';
}
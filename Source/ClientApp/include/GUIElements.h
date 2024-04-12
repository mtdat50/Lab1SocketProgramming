#pragma once
#include <string>
#include "raylib.h"
const int MAX_INPUT_CHARS = 10;

class InputBox {
public:
    InputBox();
    InputBox(const char* _label, int _x, int _y, int _width, int _height, int _fontSize);

    void Update();
    void Draw() const;
    std::string getText();

private:
    const char* label;
    Rectangle textBox;
    int fontSize;
    bool focusOn;
    char text[MAX_INPUT_CHARS + 1] = "";
    int letterCount;

    int framesCounter;
};

class Button {
public:
    Button();
    Button (const char* _label, int _x, int _y, int _width, int _height, int _fontSize);

    void Update();
    bool isHovered();
    void Draw() const;

private:
    const char* label;
    Rectangle buttonBox;
    bool hovered;
    int fontSize;
};

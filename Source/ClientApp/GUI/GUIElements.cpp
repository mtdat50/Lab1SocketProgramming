#include "../include/GUIElements.h"

InputBox::InputBox(){}

InputBox::InputBox(const char* _label, int _x, int _y, int _width, int _height, int _fontSize) {
    label = _label;
    textBox = Rectangle{(float) _x, (float) _y, (float) _width, (float) _height};
    fontSize = _fontSize;
    focusOn = false;
    text[0] = '\0';
    letterCount = 0;
    framesCounter = 0;
}

void InputBox::Draw() const {
    DrawRectangleRec(textBox, LIGHTGRAY);

    DrawText(label, textBox.x + 5, textBox.y - 30, 20, GRAY);
    DrawText(text, textBox.x + 7, textBox.y + 8, fontSize, MAROON);
    if (focusOn) {
        DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
        if (((framesCounter/20)%2) == 0 && letterCount < MAX_INPUT_CHARS) 
            DrawText("_", (int)textBox.x + 10 + MeasureText(text, fontSize), (int)textBox.y + 12, fontSize, BLACK);
    }
}

void InputBox::Update() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(GetMousePosition(), textBox))
            focusOn = true;
        else
            focusOn = false;
    }

    if (focusOn) {
        int key = GetCharPressed();
        while (key > 0) {
            if ((letterCount < MAX_INPUT_CHARS) &&
            (('A' <= key && key <= 'Z') || ('a' <= key && key <= 'z') || ('0' <= key && key <= '9') || key == '_')) {
                text[letterCount] = (char)key;
                text[letterCount + 1] = '\0'; // Add null terminator
                ++letterCount;
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            --letterCount;
            if (letterCount < 0)
                letterCount = 0;
            text[letterCount] = '\0';
        }

        framesCounter++;
    }
    else 
        framesCounter = 0;
}

std::string InputBox::getText() {
    return text;
}


Button::Button(){}

Button::Button(const char* _label, int _x, int _y, int _width, int _height, int _fontSize) {
    label = _label;
    buttonBox = Rectangle{(float) _x, (float) _y, (float) _width, (float) _height};
    fontSize = _fontSize;
    hovered  = false;
}

void Button::Update() {
    hovered = CheckCollisionPointRec(GetMousePosition(), buttonBox);
}

bool Button::isHovered() {
    return hovered;
}

void Button::Draw() const {
    Color buttonColor = hovered ? BLUE : DARKBLUE;
    DrawRectangleRec(buttonBox, buttonColor);
    DrawText(label, buttonBox.x + buttonBox.width / 2 - 26, buttonBox.y + buttonBox.height / 2 - 9, 20, GOLD);
}
#include "ChipPanel.hpp"

//Constructor
ChipPanel::ChipPanel(int& balance, int& betAmount) : x(291), realY(-130), y(realY), balance(balance), 
    betAmount(betAmount), tray("../resources/images/Tray.png"),
    chipDropSound("../resources/audio/chipDrop.wav"), errorSound("../resources/audio/error.wav") {

    loadChipTextures();
}

//Deconstructor
ChipPanel::~ChipPanel() {

    //Deallocating chip textures
    for (int i = 0; i < 5; i++) {
        delete chipTextures[i];
    }
}

//Method to animate panel's movement
void ChipPanel::update() {

    //Checking if panel needs to move
    if (realY != y) {

        //Checking if panel needs to move up or down
        if (realY < y) {
            realY += 5;

            //Checking if realY passed y
            if (realY > y) {
                realY = y;
            }

        } else {
            realY -= 5;

            //Checking if realY passed y
            if (realY < y) {
                realY = y;
            }
        }

        //return because player can't interact with panel if it's moving
        return;
    }

    registerBets();
}

//Method to draw panel
void ChipPanel::draw() const {

    //Drawing chip tray
    tray.Draw(raylib::Vector2(x, realY), 0.0f, 3.0f);

    //Getting mouse position
    raylib::Vector2 mouse = GetMousePosition();

    //Drawing chips
    for (int i = 0; i < 5; i++) {

        //Getting chip's position
        raylib::Vector2 chipPosition(x + 33 + (i * 78), realY + 33);

        //Checking if mouse is on chip, if so draw chip with a dark gray tint
        if (CheckCollisionPointRec(mouse, raylib::Rectangle(chipPosition.x, chipPosition.y, 63, 63))) {

            //Checking if mouse is pressed, if so draw chip with grey tint
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                chipTextures[i]->Draw(chipPosition, 0.0f, 3.0f, GRAY);
            } else {
                chipTextures[i]->Draw(chipPosition, 0.0f, 3.0f, DARKGRAY);
            }

        } else {
            chipTextures[i]->Draw(chipPosition, 0.0f, 3.0f); 
        }
    }
}

//Method to check if user pressed any of the chips
void ChipPanel::registerBets() {

    //Getting mouse position
    raylib::Vector2 mouse = GetMousePosition();

    //Looping through chips
    for (int i = 0; i < 5; i++) {

        //Getting chip's position
        raylib::Vector2 chipPosition(x + 33 + (i * 78), realY + 33);

        //Checking if mouse is not on chip
        if (!CheckCollisionPointRec(mouse, raylib::Rectangle(chipPosition.x, chipPosition.y, 63, 63))) {
            continue;
        }

        // Checking if mouse is not released
        if (!IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            continue;
        }

        //Getting chip's value
        int bet;

        switch (i) {
            case 0:
                bet = 5;
                break;
            case 1:
                bet = 10;
                break;
            case 2:
                bet = 25;
                break;
            case 3:
                bet = 50;
                break;
            case 4:
                bet = 100;
        }

        //Checking if player can afford bet
        if (betAmount + bet > balance) {
            errorSound.Play();
            return;
        }

        //Updating betAmount
        betAmount += bet;
        chipDropSound.Play();
        return;
    }
}

//Method to load the 5 chip textures into an array
void ChipPanel::loadChipTextures() {

    //Getting textures files' paths
    FilePathList files = LoadDirectoryFiles("../resources/images/chips");

    //Creating the 5 chip textures
    for (int i = 0; i < 5; i++) {
        chipTextures[i] = new raylib::Texture2D(files.paths[i]);
    }
}

//Set panel's vertical location to be on screen
void ChipPanel::show(){
    y = 0;
}

//Set panel's vertical location to be off screen
void ChipPanel::hide(){
    y = -126;
}

//Method to check if chipPanel is currently moving
bool ChipPanel::isMoving() const {
    return realY != y;
}
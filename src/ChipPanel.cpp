#include "ChipPanel.hpp"

//Constructor
ChipPanel::ChipPanel(int& balance, int& bet)
 : x(291), realY(-130), y(realY), balance(balance), bet(bet), tray("../resources/images/tray.png") {

    loadChipTextures();
}

//Deconstructor
ChipPanel::~ChipPanel() {
    for (int i = 0; i < 5; i++) {
        delete chipTextures[i];
    }
}

//Method to animate panel's movement and to process user clicks
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

    //Checking for user clicks
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        show();
    }
}

//Method to draw panel
void ChipPanel::draw() const {

    //Drawing chip tray
    tray.Draw(raylib::Vector2 {x, realY}, 0.0f, 3.0f);

    //Getting mouse position
    raylib::Vector2 mouse = GetMousePosition();

    //Drawing chips
    for (int i = 0; i < 5; i++) {

        //Getting chip's position
        raylib::Vector2 chipPosition(x + 33 + (i * 78), realY + 33);

        //Checking if mouse is on chip, if so draw chip with a gray tint
        if (CheckCollisionPointRec(mouse, raylib::Rectangle(chipPosition.x, chipPosition.y, 63, 63))) {
            chipTextures[i]->Draw(chipPosition, 0.0f, 3.0f, GRAY);
        } else {
            chipTextures[i]->Draw(chipPosition, 0.0f, 3.0f);
        }
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
    y = -100;
}
#include "GamePanel.hpp"

//Constructor
GamePanel::GamePanel(int windowWidth, int windowHeight)
 : WIDTH(windowWidth), HEIGHT(windowHeight), background("../resources/images/table.png"), state(waitingForAction) {

    loadCardTextures();
}

GamePanel::~GamePanel() {

    //Deallocating card textures
    for (int i = 0; i < 53; i++) {
        delete cardTextures[i];
    }

    //Deallocating cards
    for (Card* card: cards) {
        delete card;
    }
}

//Method to update game logic and components
void GamePanel::update() {

    //Updating cards
    for (Card* card: cards) {
        card->update();
    }
}

//Method to draw a frame
void GamePanel::draw() {

    background.Draw(0, 0);

    if (state == inMenu) {
        drawMenu();
        return;
    }

    //Drawing cards
    for (Card* card: cards) {
        card->draw();
    }


    DrawFPS(0, 0);
}

//Method to draw menu
void GamePanel::drawMenu() {

}

//Method to load the 53 card textures into an array
void GamePanel::loadCardTextures() {

    //Getting texture files' paths
    FilePathList files = LoadDirectoryFiles("../resources/images/cards");

    //Creating the 53 card textures (this needs the images/cards directory to be organized in alphabetic order)
    for (int i = 0; i < 53; i++) {
        cardTextures[i] = new raylib::Texture2D(files.paths[i]);
    }
}
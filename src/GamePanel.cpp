#include "GamePanel.hpp"

GamePanel::GamePanel(int windowWidth, int windowHeight)
 : WIDTH(windowWidth), HEIGHT(windowHeight), background("../resources/images/table.png"), betButton(0, 0, 100, 250, "../resources/images/buttons/BET.png"),
   hitButton(0, 0, 924, 445, "../resources/images/buttons/HIT.png"), standButton(0, 0, 824, 445, "../resources/images/buttons/STAND.png"),
   splitButton(0, 0, 924, 390, "../resources/images/buttons/SPLIT.png"), state(waitingForAction), chipPanel(balance, bet) {

    betButton.show();
    hitButton.show();
    standButton.show();
    splitButton.show();
    
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

    //Updating buttons
    betButton.update();
    hitButton.update();
    standButton.update();
    splitButton.update();

    chipPanel.update();
}

//Method to draw a frame
void GamePanel::draw() {

    if (state == inMenu) {
        drawMenu();
        return;
    }

    background.Draw(0, 0);

    //Drawing cards
    for (Card* card: cards) {
        card->draw();
    }

    //Drawing buttons
    betButton.draw();
    hitButton.draw();
    standButton.draw();
    splitButton.draw();

    chipPanel.draw();
    DrawFPS(0, 0);
}

//Method to draw menu
void GamePanel::drawMenu() {
    background.Draw(0, 0);
}

//Method to load the 53 card textures into an array
void GamePanel::loadCardTextures() {

    //Getting textures files' paths
    FilePathList files = LoadDirectoryFiles("../resources/images/cards");

    //Creating the 53 card textures (this needs the images/cards directory to be organized in alphabetic order)
    for (int i = 0; i < 53; i++) {
        cardTextures[i] = new raylib::Texture2D(files.paths[i]);
    }
}
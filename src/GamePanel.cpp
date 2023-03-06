#include "GamePanel.hpp"

GamePanel::GamePanel(int windowWidth, int windowHeight)
 : WIDTH(windowWidth), HEIGHT(windowHeight), textures(new raylib::Texture2D*[53]), background("../resources/images/table.png"), state(waitingForAction) {

    loadCardTextures();
}

void GamePanel::update() {

    //Updating cards
    for (Card card: cards) {
        card.update();
    }
}

void GamePanel::draw() {

    background.Draw(0, 0);

    if (state == inMenu) {
        drawMenu();
        return;
    }

    //Drawing cards
    for (Card card: cards) {
        card.draw();
    }


    DrawFPS(0, 0);
}

void GamePanel::drawMenu() {

}

//Method to load the 53 card textures into an array
void GamePanel::loadCardTextures() {

    //Getting texture files' paths
    FilePathList files = LoadDirectoryFiles("../resources/images/cards");

    //Creating the 52 card textures (this needs the images/cards directory to be organized in alphabetic order)
    for (int i = 0; i < 53; i++) {
        textures[i] = new raylib::Texture2D(files.paths[i]);
    }
}
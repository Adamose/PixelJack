#include "GamePanel.hpp"

GamePanel::GamePanel(int windowWidth, int windowHeight) : WIDTH(windowWidth), HEIGHT(windowHeight), textures(new raylib::Texture2D*[53]), card(5, 0, 1000, 100, 1000, 100, textures) {

    loadCardTextures();

}

void GamePanel::update() {
    card.update();
}

void GamePanel::draw() {

    ClearBackground(PURPLE);
    card.draw();
    DrawFPS(100, 100);
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
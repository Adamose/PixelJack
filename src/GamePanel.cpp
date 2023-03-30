#include "GamePanel.hpp"

GamePanel::GamePanel()
 : background("../resources/images/table.png"), betButton(1024, 445, 924, 445, "../resources/images/buttons/BET.png"),
   hitButton(924, 500, 924, 445, "../resources/images/buttons/HIT.png"), standButton(824, 500, 824, 445, "../resources/images/buttons/STAND.png"),
   splitButton(1024, 390, 924, 390, "../resources/images/buttons/SPLIT.png"), font("../resources/misc/monobit.ttf", 256), 
   balancePanel("../resources/images/BalancePanel.png"), state(WAITING_FOR_BET), chipPanel(balance, betAmount), balance(1000) {

    chipPanel.show();
    betButton.show();
    
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

    //Updating components
    betButton.update();
    hitButton.update();
    standButton.update();
    splitButton.update();
    chipPanel.update();

    switch(state) {
        case IN_MENU:
            break;

        case WAITING_FOR_BET:
            waitingForBet();
            break;

        case WAITING_FOR_MOVE:
            waitingForMove();
            break;
    }
}

//Method to draw a frame
void GamePanel::draw() {

    //Updating game state
    update();
    
    if (state == IN_MENU) {
        drawMenu();
        return;
    }

    background.Draw(0, 0);

    //Drawing cards
    for (Card* card: cards) {
        card->draw();
    }

    //Drawing components
    betButton.draw();
    hitButton.draw();
    standButton.draw();
    splitButton.draw();
    chipPanel.draw();
   
    //Drawing balance
    balancePanel.Draw(884, 0);
    const std::string balanceString = "$" + std::to_string(balance);
    int width = font.MeasureText(balanceString, 45.0f, 5.0f).GetX();
    font.DrawText(balanceString, raylib::Vector2(958 - width / 2, -10), 45.0f, 5.0f);

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

//Method called when the user pressed the bet button
void GamePanel::bet() {

    //Moving chip panel and bet button off screen
    chipPanel.hide();
    betButton.hide();

    //Updating balance
    balance -= betAmount;

}

void GamePanel::waitingForBet() {

    //Checking if bet button was pressed
    if (betButton.isPressed()) {
        bet();
    }

}

//Method called when the user pressed the hit button
void GamePanel::hit() {

}

//Method called when the user pressed the stand button
void GamePanel::stand() {

}

//Method called when the user pressed the split button
void GamePanel::split() {

}

void GamePanel::waitingForMove() {

    if (standButton.isPressed()) {

    }
}
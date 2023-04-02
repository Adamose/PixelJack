#include "GamePanel.hpp"

GamePanel::GamePanel() : background("../resources/images/table.png"), betButton(1024, 445, 924, 445, "../resources/images/buttons/BET.png"),
    hitButton(924, 545, 924, 445, "../resources/images/buttons/HIT.png"), standButton(824, 545, 824, 445, "../resources/images/buttons/STAND.png"),
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

    //Checking for button presses (button handlers are async)
    if (betButton.isPressed()) std::thread (bet, this).detach();
    if (hitButton.isPressed()) std::thread (hit, this).detach();
    if (standButton.isPressed()) std::thread(stand, this).detach();
    if (splitButton.isPressed()) std::thread(split, this).detach();

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
    std::string balanceString = "$" + std::to_string(balance);
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

//Method called when the user pressed the bet button (runs on side thread)
void GamePanel::bet() {

    //Updating balance
    balance -= betAmount;

    //Moving chip panel and bet button off screen
    chipPanel.hide();
    betButton.hide();
    while (chipPanel.isMoving()) {}

    Card* temporaryCard;

    //Drawing dealer's first card
    temporaryCard = new Card(0, 430, 40, cardTextures);
    cards.push_back(temporaryCard);
    dealerHand.push_back(temporaryCard);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    //Drawing player's first card
    temporaryCard = new Card(0, 430, 300, cardTextures);
    cards.push_back(temporaryCard);
    playerHandOne.push_back(temporaryCard);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    //Drawing dealer's second card
    temporaryCard = new Card(0, 510, 40, cardTextures);
    cards.push_back(temporaryCard);
    dealerHand.push_back(temporaryCard);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    //Drawing player's second card
    temporaryCard = new Card(0, 510, 300, cardTextures);
    cards.push_back(temporaryCard);
    playerHandOne.push_back(temporaryCard);
    while (temporaryCard->isMoving()) {}

    //Showing action buttons
    hitButton.show();
    standButton.show();

    //Checking if split button has to be shown
    if (playerHandOne[0]->getValue() == playerHandOne[1]->getValue()) {
        splitButton.show();
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
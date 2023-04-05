#include "GamePanel.hpp"

GamePanel::GamePanel() : background("../resources/images/table.png"), betButton(1024, 445, 924, 445, "../resources/images/buttons/BET.png"),
    hitButton(924, 545, 924, 445, "../resources/images/buttons/HIT.png"), standButton(824, 545, 824, 445, "../resources/images/buttons/STAND.png"),
    splitButton(1024, 390, 924, 390, "../resources/images/buttons/SPLIT.png"), font("../resources/misc/monobit.ttf", 256), betAmount(0), balance(1000),
    balancePanel("../resources/images/BalancePanel.png"), cardDrawSound("../resources/audio/cardDraw.wav"), chipPanel(balance, betAmount), 
    chipsDropSound("../resources/audio/chipsDrop.wav"), cardSlideSound("../resources/audio/cardSlide.wav"), errorSound("../resources/audio/error.wav"), 
    cardFlipSound("../resources/audio/cardFlip.wav"), threadAvailable(true) {

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

    //Checking if we can handle buttons presses (handler thread avaible)
    if (threadAvailable) {

        //Checking for button presses (button handlers are async)
        if (betButton.isPressed()) { threadAvailable = false; std::thread (bet, this).detach(); return; }
        if (hitButton.isPressed()) { threadAvailable = false; std::thread (hit, this).detach(); return; }
        if (standButton.isPressed()) { threadAvailable = false; std::thread(stand, this).detach(); return; }
        if (splitButton.isPressed()) { threadAvailable = false; std::thread(split, this).detach(); return; }
    }
}

//Method to draw a frame
void GamePanel::draw() {

    //Updating game state
    update();

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
   
    //Drawing hand values
    drawHandValues();

    //Drawing balance
    balancePanel.Draw(884, 0);
    std::string balanceString = "$" + std::to_string(balance);
    int width = font.MeasureText(balanceString, 45.0f, 5.0f).GetX();
    font.DrawText(balanceString, raylib::Vector2(958 - width / 2, -10), 45.0f, 5.0f);

    font.DrawText(std::to_string(betAmount), raylib::Vector2(900, 100), 45.0f, 5.0f);

    DrawFPS(0, 0);
}

//Method to draw menu
void GamePanel::drawMenu() const {
    background.Draw(0, 0);
}

//Method called when the user pressed the bet button (runs on side thread)
void GamePanel::bet() {

    //Checking if bet is invalid
    if (betAmount == 0 || betAmount > balance) {
        errorSound.Play();
        threadAvailable = true;
        return;
    }

    //Updating balance
    balance -= betAmount;

    //Moving chip panel and bet button off screen
    chipPanel.hide();
    betButton.hide();
    while (chipPanel.isMoving()) {}

    Card* temporaryCard;

    //Drawing dealer's first card
    temporaryCard = new Card(getCardId(), 441, 40, cardTextures);
    cards.push_back(temporaryCard);
    dealerHand.push_back(temporaryCard);
    cardDrawSound.PlayMulti();
    cardSlideSound.PlayMulti();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    //Drawing player's first card
    temporaryCard = new Card(getCardId(), 479, 300, cardTextures);
    cards.push_back(temporaryCard);
    playerHandOne.push_back(temporaryCard);
    cardDrawSound.PlayMulti();
    cardSlideSound.PlayMulti();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    //Drawing dealer's second card
    temporaryCard = new Card(getCardId(), 517, 40, cardTextures);
    temporaryCard->setFacedown(true);
    cards.push_back(temporaryCard);
    dealerHand.push_back(temporaryCard);
    cardDrawSound.PlayMulti();
    cardSlideSound.PlayMulti();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    //Drawing player's second card
    temporaryCard = new Card(getCardId(), 501, 283, cardTextures);
    cards.push_back(temporaryCard);
    playerHandOne.push_back(temporaryCard);
    cardDrawSound.PlayMulti();
    cardSlideSound.PlayMulti();
    while (temporaryCard->isMoving()) {}

    //Check if player got a blackjack
    if (playerHandOne[0]->getValue() + playerHandOne[1]->getValue() == 11) {
        //printf("blackjack");
    }

    //Showing action buttons
    hitButton.show();
    standButton.show();

    //Checking if split button has to be shown
    if (playerHandOne[0]->getValue() == playerHandOne[1]->getValue()) {
        //splitButton.show();
    }

    threadAvailable = true;
}

//Method called when the user pressed the hit button
void GamePanel::hit() {

    //Draw a card
    Card* newCard = new Card(getCardId(), 479 + (playerHandOne.size() * 22), 300 - (playerHandOne.size() * 17), cardTextures);
    cards.push_back(newCard);
    playerHandOne.push_back(newCard);
    cardDrawSound.PlayMulti();
    cardSlideSound.PlayMulti();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    //Check if player lost
    if (getHandValue(playerHandOne) > 21) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        clearGame();
    }

    threadAvailable = true;
}

//Method called when the user pressed the stand button
void GamePanel::stand() {

    //Reveal dealer card
    cardFlipSound.Play();
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    dealerHand[1]->setFacedown(false);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    //Dealer hits until hand value > 16
    while (true) {

        //Checking if dealer doesn't need to draw anymore
        if (getHandValue(dealerHand) > 17 || (getHandValue(dealerHand) == 17 && !handHasAce(dealerHand))) {
            break;
        }

        //Draw a card
        Card* newCard = new Card(getCardId(), 1100, -100, cardTextures);
        cards.push_back(newCard);
        dealerHand.push_back(newCard);
        cardDrawSound.Play();
        centerDealerHand();
        cardSlideSound.Play();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); 
    }

    //Check if won or tied
    if (getHandValue(dealerHand) > 21 || getHandValue(playerHandOne) > getHandValue(dealerHand)) {
        balance += betAmount * 2;
    } else if (getHandValue(playerHandOne) == getHandValue(dealerHand)) {
        balance += betAmount;
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    clearGame();

    threadAvailable = true;
}

//Method called when the user pressed the split button
void GamePanel::split() {

    threadAvailable = true;
}

//Method to reset variables to start a new game
void GamePanel::clearGame() {

    //Moving cards off table
    cardSlideSound.Play();

    for (Card* card: cards) {
        card->setLocation(-141, -100);
    }
    
    //Hiding buttons
    standButton.hide();
    hitButton.hide();
    splitButton.hide();
    
    //Wait for all elements to be off screen
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    //Showing chipPanel and betButton
    chipPanel.show();
    betButton.show();

    //Resetting  variables
    betAmount = 0;
    dealerHand.clear();
    playerHandOne.clear();
    playerHandTwo.clear();
    cards.clear();
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

//Method to get a random number between 0 and 51
int GamePanel::getCardId() const {

    //Static variables only initialized once during program execution
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 51);

    return dis(gen);
}

//Method to get hand's value
int GamePanel::getHandValue(const std::vector<Card*>& hand) const {

    int value = 0;
    int containsAce = false;

    //Caculating value ignoring first occuring ace
    for (Card* card: hand) {

        //Checking if card is first occuring ace
        if (!containsAce && card->getValue() == 1) {
            containsAce = true;
            continue;
        }

        value += card->getValue();
    }

    //Checking if we didn't ignore an ace
    if (!containsAce) {
        return value;
    }

    //Deciding wether to add 1 or 11 for the ace we ignored
    if (value < 11) {
        value += 11;
    } else {
        value++;
    }

    return value;
}

//Method to get hand's values (soft vs hard ace)
raylib::Vector2 GamePanel::getHandValues(const std::vector<Card*>& hand) const {

    int value = 0;
    int containsAce = false;

    //Caculating value ignoring first occuring ace
    for (Card* card: hand) {

        //Checking if card is first occuring ace
        if (!containsAce && card->getValue() == 1) {
            containsAce = true;
            continue;
        }

        value += card->getValue();
    }

    //Checking if we didn't ignore an ace
    if (!containsAce) {
        return raylib::Vector2(value, value);
    }

    return raylib::Vector2(value + 1, value + 11);
}

//Method to change dealer's cards' postitions to be centered
void GamePanel::centerDealerHand() {

    //Finding width of dealer's hand
    int width = (dealerHand.size() * 76) - 10;

    //Finding position of left-most card
    int startingX = 512 - (width / 2);

    //Setting cards' new locations
    for (Card* card: dealerHand) {
        card->setLocation(startingX, 40);
        startingX += 76;
    }
}

//Method to check wether a hand has atleast one ace
bool GamePanel::handHasAce(const std::vector<Card*>& hand)  const {
    for (Card* card: hand) {
        if (card->getValue() == 1) {
            return true;
        }
    }

    return false;
}

//Method to draw hands' values
void GamePanel::drawHandValues() const {

    std::string text;
    int width;

    if (!dealerHand.empty()) {
        raylib::Vector2 values = getHandValues(dealerHand);
        
        if (values.x == values.y || values.y > 21) {
            text = std::to_string( (int) values.y);
        } else {
            text = std::to_string( (int) values.x) + "/" + std::to_string( (int) values.y);
        }

        width = font.MeasureText(text, 35.0f, 1.0f).GetX();
        font.DrawText(text, raylib::Vector2(512 - (width / 2), 200), 35.0f, 1.0f);
    }

    if (!playerHandOne.empty()) {
        raylib::Vector2 values = getHandValues(playerHandOne);
        
        if (values.x == values.y || values.y > 21) {
            text = std::to_string( (int) values.x);
        } else {
            text = std::to_string( (int) values.x) + "/" + std::to_string( (int) values.y);
        }

        width = font.MeasureText(text, 35.0f, 1.0f).GetX();
        font.DrawText(text, raylib::Vector2(512 - (width / 2), 390), 35.0f, 1.0f);
    }

    if (!playerHandTwo.empty()) {
        raylib::Vector2 values = getHandValues(playerHandTwo);
        
        if (values.x == values.y || values.y > 21) {
            text = std::to_string( (int) values.x);
        } else {
            text = std::to_string( (int) values.x) + "/" + std::to_string( (int) values.y);
        }

        width = font.MeasureText(text, 35.0f, 1.0f).GetX();
        font.DrawText(text, raylib::Vector2(512 - (width / 2), 390), 35.0f, 1.0f);
    }
}
#include "GamePanel.hpp"

GamePanel::GamePanel() : background("../resources/images/table.png"), betButton(1024, 445, 924, 445, "../resources/images/buttons/BET.png"),
    hitButton(924, 545, 924, 445, "../resources/images/buttons/HIT.png"), standButton(824, 545, 824, 445, "../resources/images/buttons/STAND.png"),
    splitButton(1024, 390, 924, 390, "../resources/images/buttons/SPLIT.png"), font("../resources/misc/monobit.ttf", 256), betAmount(0), balance(1000),
    balancePanel("../resources/images/BalancePanel.png"), cardDrawSound("../resources/audio/cardDraw.wav"), chipPanel(balance, betAmount), inGame(false), 
    chipsDropSound("../resources/audio/chipsDrop.wav"), cardSlideSound("../resources/audio/cardSlide.wav"), errorSound("../resources/audio/error.wav"), 
    cardFlipSound("../resources/audio/cardFlip.wav"), betPanel("../resources/images/BetPanel.png"), threadAvailable(true), drawHands(false), 
    playerSplit(false), handOneActive(true), handOneHasBlackjack(false) {

    loadCardTextures();

    messageBoard.show();
    messageBoard.setTitle(0);

    //Waiting for user to click on screen to start game (async to not prevent drawing)
    std::thread([this] {  

        while (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {}
        this->messageBoard.hide();
        while (this->messageBoard.getX() != -310) {}
        this->chipPanel.show();
        this->betButton.show();
        this->messageBoard.setTitle(-1);
        this->messageBoard.show();

    }).detach();
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
    messageBoard.update();

    //Checking if we can handle buttons presses (handler thread avaible)
    if (threadAvailable) {

        //Checking for button presses (button handlers are async)
        if (betButton.isPressed()) { threadAvailable = false; std::thread([this] { bet(); }).detach(); return; }
        if (hitButton.isPressed()) { threadAvailable = false; std::thread([this] { hit(); }).detach(); return; }
        if (standButton.isPressed()) { threadAvailable = false; std::thread([this] { stand(); }).detach(); return; }
        if (splitButton.isPressed()) { threadAvailable = false; std::thread([this] { split(); }).detach(); return; }
    }

    //Checking if player pressed betPanel
    if (!inGame) {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), raylib::Rectangle(0, 0, 154, 36))) {
            if (betAmount == 0) {
                errorSound.Play();
            } else {
                betAmount = 0;
            }
        }
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
    messageBoard.draw();
   
    //Drawing hand values
    if (drawHands) drawHandValues();

    //Drawing balance and betAmount
    drawBetBalance();
}

//Method to draw menu
void GamePanel::drawMenu() const {
    background.Draw(0, 0);
}

//Method called when the user pressed the bet button (runs on side thread)
void GamePanel::bet() {
    messageBoard.hide();

    //Checking if bet is invalid
    if (betAmount == 0 || betAmount > balance) {
        errorSound.Play();
        threadAvailable = true;
        return;
    }

    //Updating balance
    inGame = true;
    balance -= betAmount;

    //Moving chip panel and bet button off screen
    chipPanel.hide();
    betButton.hide();
    messageBoard.hide();
    while (chipPanel.isMoving()) {}

    Card* temporaryCard;

    //Drawing dealer's first card
    temporaryCard = new Card(getCardId(), 441, 40, cardTextures);
    cards.push_back(temporaryCard);
    dealerHand.push_back(temporaryCard);
    cardDrawSound.Play();
    cardSlideSound.Play();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    //Drawing player's first card
    temporaryCard = new Card(getCardId(), 479, 300, cardTextures);
    cards.push_back(temporaryCard);
    playerHandOne.push_back(temporaryCard);
    cardDrawSound.Play();
    cardSlideSound.Play();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    drawHands = true;
    
    //Drawing dealer's second card
    temporaryCard = new Card(getCardId(), 517, 40, cardTextures);
    temporaryCard->setFacedown(true);
    cards.push_back(temporaryCard);
    dealerHand.push_back(temporaryCard);
    cardDrawSound.Play();
    cardSlideSound.Play();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    //Drawing player's second card
    temporaryCard = new Card(getCardId(), 501, 283, cardTextures);
    cards.push_back(temporaryCard);
    playerHandOne.push_back(temporaryCard);
    cardDrawSound.Play();
    cardSlideSound.Play();
    while (temporaryCard->isMoving()) {}

    //Check if player got a blackjack
    if (getHandValue(playerHandOne) == 21) {
        
        //Reveal dealer card
        cardFlipSound.Play();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        dealerHand[1]->setFacedown(false);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        //Check if dealer also got blackjack
        if (getHandValue(dealerHand) == 21) {
            messageBoard.setMessage(0);
            messageBoard.setTitle(3);
            balance += betAmount;

        } else {
            int profit = (betAmount * 3) / 2;
            balance += profit + betAmount;
            chipsDropSound.Play();
            messageBoard.setMessage(profit);
            messageBoard.setTitle(4);
        }

        messageBoard.show();
        while (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {}
        clearGame();

        threadAvailable = true;
    }

    //Showing action buttons
    hitButton.show();
    standButton.show();

    //Checking if split button has to be shown
    if (playerHandOne[0]->getValue() == playerHandOne[1]->getValue() && balance >= betAmount) {
        splitButton.show();
    }

    threadAvailable = true;
}

//Method called when the user pressed the hit button
void GamePanel::hit() {
    splitButton.hide();

    std::vector<Card*>* hand = &playerHandOne;
    int x = 479;

    //Checking if we're hitting a split hand
    if (playerSplit) {
        if (handOneActive) {
            x = 562;
        } else {
            x = 396;
            hand = &playerHandTwo;
        }
    }

    //Draw a card
    Card* newCard = new Card(getCardId(), x + ((*hand).size() * 22), 300 - ((*hand).size() * 17), cardTextures);
    cards.push_back(newCard);
    (*hand).push_back(newCard);
    cardDrawSound.Play();
    cardSlideSound.Play();
    std::this_thread::sleep_for(std::chrono::milliseconds(750));

    //Check if player lost
    if (getHandValue((*hand)) > 21) {

        //Checking if active split hand should now be hand two
        if (playerSplit && hand == &playerHandOne) {
            handOneActive = false;
        } else {
            
            if (playerSplit) {

                //Check if right hand is still playing
                if (getHandValue(playerHandOne) <= 21) {

                    if (!handOneHasBlackjack) {
                        stand();
                        return;
                    }
                    
                    messageBoard.setTitle(12);
                    messageBoard.setMessage(betAmount / 2);
                    balance += betAmount / 2;
                    balance += betAmount;

                } else {
                    messageBoard.setTitle(6);
                    messageBoard.setMessage(betAmount * -2);
                }

            } else {
                messageBoard.setTitle(2);
                messageBoard.setMessage(betAmount * -1);
            }

            //Reveal dealer card
            cardFlipSound.Play();
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            dealerHand[1]->setFacedown(false);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            messageBoard.show();
            while (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {}
            clearGame();
        }
    }

    //Check if player has 21
    if (getHandValue((*hand)) == 21) {

        //Checking if active split hand should now be hand two
        if (playerSplit && hand == &playerHandOne) {
            handOneActive = false;
        } else {
            stand();
            return;
        }
    }

    threadAvailable = true;
}

//Method called when the user pressed the stand button
void GamePanel::stand() {
    splitButton.hide();

    //Checking if player is standing first hand after split
    if (playerSplit && handOneActive) {

        handOneActive = false;

        //Checking if left split hand has blackjack
        if (getHandValue(playerHandTwo) == 21) {

            //Reveal dealer card
            cardFlipSound.Play();
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            dealerHand[1]->setFacedown(false);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            
            int gain = (betAmount * 3) / 2;
            balance += (betAmount * 3) / 2;
            balance += betAmount;
            chipsDropSound.Play();

            //Check if right hand won
            if (getHandValue(playerHandOne) <= 21 && (getHandValue(dealerHand) > 21 || getHandValue(playerHandOne) > getHandValue(dealerHand))) {
                 
                gain += betAmount;
                messageBoard.setTitle(11);
                messageBoard.setMessage(gain);
                balance += betAmount * 2;
            
            } else if (getHandValue(playerHandOne) == getHandValue(dealerHand)) {
                messageBoard.setTitle(13);
                messageBoard.setMessage(gain);
                balance += betAmount;

            } else {
                gain -= betAmount;
                messageBoard.setTitle(12);
                messageBoard.setMessage(gain);
            }

            messageBoard.show();
            while (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {}
            clearGame();
        }

        threadAvailable = true;
        return;
    }

    //Reveal dealer card
    cardFlipSound.Play();
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    dealerHand[1]->setFacedown(false);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    //Dealer hits until hand value > 16
    while (true) {

        //Checking if dealer doesn't need to draw anymore
        if (getHandValue(dealerHand) > 16 && !(getHandValues(dealerHand) == raylib::Vector2(7, 17))) {
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

    int gain = 0;

    //Check if handOne won or tied
    if (getHandValue(playerHandOne) <= 21 && (getHandValue(dealerHand) > 21 || getHandValue(playerHandOne) > getHandValue(dealerHand))) {
        gain += betAmount;
        balance += betAmount * 2;
        chipsDropSound.Play();
        messageBoard.setTitle(1); 
        
    } else if (getHandValue(playerHandOne) <= 21 && getHandValue(playerHandOne) == getHandValue(dealerHand)) {
        balance += betAmount;
        messageBoard.setTitle(3);
    } else {
        gain -= betAmount;
        messageBoard.setTitle(2);
    }

    //Check if handTwo won or tied
    if (playerSplit) {
        if (getHandValue(playerHandTwo) <= 21 && (getHandValue(dealerHand) > 21 || getHandValue(playerHandTwo) > getHandValue(dealerHand))) {
            gain += betAmount;
            balance += betAmount * 2;
            chipsDropSound.Play();
            
            if (handOneHasBlackjack) {
                messageBoard.setTitle(11);
            } else if (gain == betAmount * 2) {
                messageBoard.setTitle(5);
            } else if (gain == betAmount) {
                messageBoard.setTitle(8);
            } else {
                messageBoard.setTitle(7);
            }

        } else if (getHandValue(playerHandTwo) <= 21 && getHandValue(playerHandTwo) == getHandValue(dealerHand)) {
            balance += betAmount;

            if (handOneHasBlackjack) {
                messageBoard.setTitle(13);
            } else if (gain == betAmount) {
                messageBoard.setTitle(8);
            } else if (gain == 0) {
                messageBoard.setTitle(10);
            } else {
                messageBoard.setTitle(9);
            }

        } else {
            gain -= betAmount;

            if (handOneHasBlackjack) {
                messageBoard.setTitle(12);
            } else if (gain == 0) {
                messageBoard.setTitle(7);
            } else if (gain == betAmount * -1) {
                messageBoard.setTitle(9);
            } else {
                messageBoard.setTitle(6);
            }
        }

        if (handOneHasBlackjack) {
            balance += betAmount / 2;
            gain += betAmount / 2;
        }
    }
    
    messageBoard.setMessage(gain);
    messageBoard.show();
    while (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {}
    clearGame();

    threadAvailable = true;
}

//Method called when the user pressed the split button
void GamePanel::split() {

    splitButton.hide();
    playerSplit = true;
    balance -= betAmount;

    //Moving second card to second hand
    playerHandTwo.push_back(playerHandOne[1]);
    playerHandOne.pop_back();

    //Adjusting hands' positions
    playerHandOne[0]->setLocation(562, 300);
    playerHandTwo[0]->setLocation(396, 300);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    Card* temporaryCard;

    //Drawing first hand's second card
    temporaryCard = new Card(getCardId(), 584, 283, cardTextures);
    cards.push_back(temporaryCard);
    playerHandOne.push_back(temporaryCard);
    cardDrawSound.Play();
    cardSlideSound.Play();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    //Drawing second hand's second card
    temporaryCard = new Card(getCardId(), 418, 283, cardTextures);
    cards.push_back(temporaryCard);
    playerHandTwo.push_back(temporaryCard);
    cardDrawSound.Play();
    cardSlideSound.Play();
    std::this_thread::sleep_for(std::chrono::milliseconds(750));

    //Checking if right split hand has blackjack
    if (getHandValue(playerHandOne) == 21) {
        handOneHasBlackjack = true;

        //CHecking if left split hand has blackjack
        if (getHandValue(playerHandTwo) == 21) {
            balance += betAmount * 3;
            balance += betAmount * 2;
            messageBoard.setTitle(14);
            messageBoard.setMessage(betAmount * 3);

            //Reveal dealer card
            cardFlipSound.Play();
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            dealerHand[1]->setFacedown(false);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            chipsDropSound.Play();
            messageBoard.show();
            while (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {}
            clearGame();

        } else {
            stand();
        }
    }

    threadAvailable = true;
}

//Method to reset variables to start a new game
void GamePanel::clearGame() {

    cardSlideSound.Play();
    drawHands = false;

    //Moving cards off the table
    for (Card* card: cards) {
        card->setLocation(-141, -100);
    }
    
    //Hiding buttons
    standButton.hide();
    hitButton.hide();
    splitButton.hide();
    messageBoard.hide();
    
    //Wait for all elements to be off screen
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    //Freeing memory allocated to cards
    for (Card* card: cards) {
        delete card;
    }

    //Showing chipPanel and betButton
    chipPanel.show();
    betButton.show();

    //Emptying arrays
    dealerHand.clear();
    playerHandOne.clear();
    playerHandTwo.clear();
    cards.clear();

    playerSplit = false;
    handOneActive = true;
    handOneHasBlackjack = false;
    inGame = false;

    messageBoard.setTitle(-1);
    messageBoard.show();
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

    //Checking for blackjack
    if (value == 10) {
        return raylib::Vector2(21, 21);
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
        
        //Checking if dealer is hiding second card
        if (dealerHand.size() == 1 || dealerHand[1]->getFacedown()) {

            //Checking if first card is an ace
            if (dealerHand[0]->getValue() == 1) {
                text = "1/11";
            } else {
                text = std::to_string(dealerHand[0]->getValue());
            }

        } else {

            //Checking if we need to display soft and hard values
            if (values.x == values.y || values.y > 21) {
                text = std::to_string( (int) values.x);
            } else {
                text = std::to_string( (int) values.x) + "/" + std::to_string( (int) values.y);
            }
        }

        //Getting width to center text
        width = font.MeasureText(text, 35.0f, 1.0f).GetX();
        font.DrawText(text, raylib::Vector2(514 - (width / 2), 133), 35.0f, 1.0f);
    }

    if (!playerHandOne.empty()) {
        raylib::Vector2 values = getHandValues(playerHandOne);
        
        //Checking if we need to display soft and hard values
        if (values.x == values.y || values.y > 21) {
            text = std::to_string( (int) values.x);
        } else {
            text = std::to_string( (int) values.x) + "/" + std::to_string( (int) values.y);
        }

        //Getting width to center text
        int x = (playerSplit) ? 597 : 514;
        raylib::Color color = (playerSplit && handOneActive) ? GOLD : WHITE;
        width = font.MeasureText(text, 35.0f, 1.0f).GetX();
        font.DrawText(text, raylib::Vector2(x - (width / 2), 393), 35.0f, 1.0f, color);
    }

    if (!playerHandTwo.empty()) {
        raylib::Vector2 values = getHandValues(playerHandTwo);
        
        //Checking if we need to display soft and hard values
        if (values.x == values.y || values.y > 21) {
            text = std::to_string( (int) values.x);
        } else {
            text = std::to_string( (int) values.x) + "/" + std::to_string( (int) values.y);
        }

        //Getting width to center text
        raylib::Color color = (!handOneActive) ? GOLD : WHITE;
        width = font.MeasureText(text, 35.0f, 1.0f).GetX();
        font.DrawText(text, raylib::Vector2(431 - (width / 2), 393), 35.0f, 1.0f, color);
    }
}

//Method to draw betAmount and balance
void GamePanel::drawBetBalance() const {
    std::string text;
    int width;

    //Drawing balance
    balancePanel.Draw(884, 0);
    text = "$" + std::to_string(balance);
    width = font.MeasureText(text, 45.0f, 5.0f).GetX();
    font.DrawText(text, raylib::Vector2(958 - width / 2, -10), 45.0f, 5.0f);

    //Checking which betPanel texture to draw
    raylib::Rectangle betPanelRectangle(0, 0, 154, 36);
    int y = CheckCollisionPointRec(GetMousePosition(), betPanelRectangle) ? 37 : 0;

    //Drawing bet
    betPanel.Draw(raylib::Rectangle(0, y, 154, 36), betPanelRectangle, raylib::Vector2(0, 0), 0.0f);
    text = std::to_string(betAmount);
    width = font.MeasureText(text, 45.0f, 5.0f).GetX();
    font.DrawText(text, raylib::Vector2(54 - width / 2, -10), 45.0f, 5.0f);
}
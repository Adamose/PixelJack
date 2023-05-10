#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <random>
#include "../include/raylib-cpp.hpp"
#include "Card.hpp"
#include "ChipPanel.hpp"
#include "Button.hpp"

class GamePanel {

    public:
        GamePanel();
        ~GamePanel();
        void draw();

    private:
        void loadCardTextures();
        int getCardId() const;
        int getHandValue(const std::vector<Card*>& hand) const;
        raylib::Vector2 getHandValues(const std::vector<Card*>& hand) const;
        void centerDealerHand();
        bool handHasAce(const std::vector<Card*>& hand) const;
        void update();
        void drawMenu() const;
        void drawBetBalance() const;
        void drawHandValues() const;
        void clearGame();

        //Button handlers
        void bet();
        void hit();
        void stand();
        void split();

        int balance;                            //player's current balance
        int betAmount;                          //player's current bet amount
        bool playerSplit;                       //boolean used to determine if the player currently has two hands
        bool handOneActive;                     //boolean used to determine which hand is active after splitting
        bool inGame;                            //boolean used to determine if player is in a game (bet button is not on screen)
        bool threadAvailable;                   //boolean used to determine if we currently have a handler thread running
        bool drawHands;                         //boolean used to determine wether to draw hands' values
        Button betButton;
        Button hitButton;
        Button standButton;
        Button splitButton;
        ChipPanel chipPanel;                    //chipPanel used to get user's bet
        raylib::Sound cardDrawSound;
        raylib::Sound cardSlideSound;
        raylib::Sound cardFlipSound;
        raylib::Sound chipsDropSound;
        raylib::Sound errorSound;
        raylib::Font font;
        raylib::Texture2D* cardTextures[53];    //array of pointers for card textures
        std::vector<Card*> cards;               //dynamic array of card pointers
        std::vector<Card*> dealerHand;          //dynamic array of card pointers that are currently in the dealer's hand
        std::vector<Card*> playerHandOne;       //dynamic array of card pointers that are currently in the player's first hand
        std::vector<Card*> playerHandTwo;       //dynamic array of card pointers that are currently in the player's second hand
        raylib::Texture2D const background;     //texture to draw as background
        raylib::Texture2D const balancePanel;   //texture to draw behind balance
        raylib::Texture2D const betPanel;       //texture to draw behind betAmount
    
};

#endif
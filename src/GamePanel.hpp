#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include <vector>
#include <string>
#include "raylib-cpp.hpp"
#include "Card.hpp"
#include "ChipPanel.hpp"
#include "Button.hpp"

//inMenu: showing menu waiting for player to press play
//waitingForBet: waiting for player to press bet
//waitingForAction: waiting for player to select and action for their hand
enum State { IN_MENU, WAITING_FOR_BET, WAITING_FOR_MOVE };

class GamePanel {

    public:
        GamePanel();
        ~GamePanel();

        void draw();                           //Drawing one frame

    private:
        void loadCardTextures();
        void update();                         //Updating game components
        void waitingForBet();
        void waitingForMove();
        void drawMenu();

        //Button handlers
        void bet();
        void hit();
        void stand();
        void split();

        int balance;
        int betAmount;
        Button betButton;
        Button hitButton;
        Button standButton;
        Button splitButton;
        raylib::Font font;
        raylib::Texture2D* cardTextures[53];    //array of pointers for card textures
        std::vector<Card*> cards;               //dynamic array of card pointers
        std::vector<Card*> dealerHand;          //dynamic array of card pointers that are currently in the dealer's hand
        std::vector<Card*> playerHandOne;       //dynamic array of card pointers that are currently in the player's first hand
        std::vector<Card*> playerHandTwo;       //dynamic array of card pointers that are currently in the player's second hand
        State state;                            //current state of the program
        ChipPanel chipPanel;                    //chipPanel used to get user's bet
        raylib::Texture2D const background;     //texture to draw as background
        raylib::Texture2D const balancePanel;   //texture to draw behind balance
    
};

#endif
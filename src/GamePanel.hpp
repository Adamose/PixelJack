#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include <vector>
#include "raylib-cpp.hpp"
#include "Card.hpp"
#include "ChipPanel.hpp"

//inMenu: showing menu waiting for player to press play
//waitingForBet: waiting for player to press bet
//waitingForAction: waiting for player to select and action for their hand
enum State { inMenu, waitingForBet, waitingForAction };

class GamePanel {

    public:
        GamePanel(int windowWidth, int windowHeight);
        ~GamePanel();

        void update(); 
        void draw();

        const int WIDTH;
        const int HEIGHT;

    private:
        void loadCardTextures();
        void drawMenu();


        int balance;
        int bet;
        raylib::Texture2D* cardTextures[53];    //array of pointers for card textures
        std::vector<Card*> cards;               //dynamic array of card pointers
        State state;
        ChipPanel chipPanel;


        raylib::Texture2D const background;     //texture to draw as background
    
};

#endif
#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include <vector>
#include "raylib-cpp.hpp"
#include "Card.hpp"

//inMenu: showing menu waiting for player to press play
//waitingForBet: waiting for player to press bet
//waitingForAction: waiting for player to select and action for their hand
enum State { inMenu, waitingForBet, waitingForAction };

class GamePanel {

    public:
        GamePanel(int windowWidth, int windowHeight);

        void update(); 
        void draw();

        const int WIDTH;
        const int HEIGHT;

    private:
        void loadCardTextures();
        void drawMenu();

        std::vector<Card> cards;
        State state;

        raylib::Texture2D const background;     //texture to draw as background
        raylib::Texture2D** const textures;     //pointer to a dynamically allocated array of card textures
    
};

#endif
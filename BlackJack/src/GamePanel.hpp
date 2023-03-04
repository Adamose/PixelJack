#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include "raylib-cpp.hpp"
#include "Card.hpp"

class GamePanel {

    public:
        GamePanel(int windowWidth, int windowHeight);

        void update(); 
        void draw();

        const int WIDTH;
        const int HEIGHT;

    private:
        void loadCardTextures();
        raylib::Texture2D** const textures;      //pointer to a dynamically allocated array of card textures
        Card card;

};

#endif
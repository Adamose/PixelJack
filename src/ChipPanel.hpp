#ifndef CHIPPANEL_H
#define CHIPPANEL_H

#include "raylib-cpp.hpp"

//Class for panel containing chip buttons for user to select bet amount
class ChipPanel {

    public:
        ChipPanel(int& balance, int& betAmount);
        ~ChipPanel();

        void update();
        void draw() const;
        void show();
        void hide();

    private:
        void loadChipTextures();

        int& balance;                           //user's balance referenced from GamePanel that created this ChipPanel
        int& betAmount;                               //user's current bet amount referenced from GamePanel that created this ChipPanel
        int realY;                              //panel's current vertical location
        int y;                                  //vertical location to be/move to
        raylib::Texture2D* chipTextures[5];     //array of chip texture pointers
        raylib::Texture2D tray;                 //tray texture
        
        const int x;

};

#endif
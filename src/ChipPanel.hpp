#ifndef CHIPPANEL_H
#define CHIPPANEL_H

#include "../include/raylib-cpp.hpp"

//Class for panel containing chip buttons for user to select bet amount
class ChipPanel {

    public:
        ChipPanel(int& balance, int& betAmount);
        ~ChipPanel();

        void update();
        void draw() const;
        void show();
        void hide();
        bool isMoving() const;

    private:
        void loadChipTextures();
        void registerBets();

        int& balance;                           //user's balance referenced from GamePanel that created this ChipPanel
        int& betAmount;                         //user's current bet amount referenced from GamePanel that created this ChipPanel
        int realY;                              //panel's current vertical location
        int y;                                  //vertical location to be/move to
        raylib::Texture2D* chipTextures[5];     //array of chip texture pointers
        raylib::Texture2D tray;                 //tray texture
        raylib::Sound chipDropSound;            //Sound used when selecting a chip
        raylib::Sound errorSound;               //Sound used for invalid bet increases
        
        const int x;

};

#endif
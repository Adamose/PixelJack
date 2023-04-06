#ifndef CARD_H
#define CARD_H

#include "raylib-cpp.hpp"
#include <algorithm>

class Card {

    public:
        Card(int id, int x, int y, raylib::Texture2D** textures);

        void update();
        void draw() const;
        void setLocation(int x, int y);
        bool isMoving() const;
        int getValue() const;
        void setFacedown(bool boolean);
        bool getFacedown() const;

    private:
        const int id;                           //id used to pick the card texture to draw from 52 possible cards (id 0 is the back of a card)
        const int value;                        //value of card in terms of blackjack rule (face cards are 10, ace is 1/11)
        bool facedown;                          //boolean used to determine if card should be drawned face down
        raylib::Texture2D** const textures;     //array of pointers for card textures
        raylib::Vector2 realLocation;           //card's current location (starts at top right)
        raylib::Vector2 location;               //location to be/move to

};

#endif
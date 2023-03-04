#ifndef CARD_H
#define CARD_H

#include "raylib-cpp.hpp"

class Card {

    public:
        Card(int id, int value, int x, int y, int initialX, int initialY, raylib::Texture2D** textures);

        void update();
        void draw() const;
        void setX(int x);
        void setY(int y);

    private:
        const int id;                           //id used to pick the card texture to draw from 52 possible cards (id 0 is the back of a card)
        const int value;                        //value of card in terms of blackjack rule (face cards are 10, ace is 1/11)
        raylib::Texture2D** const textures;     //pointer to a dynamically allocated array of texture pointers

        raylib::Vector2 realLocation;           //card's current location (starts at top right)
        raylib::Vector2 location;               //location to be/move to

};

#endif
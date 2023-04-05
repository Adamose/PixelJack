#ifndef BUTTON_H
#define BUTTON_H

#include "raylib-cpp.hpp"
#include <cmath>
#include <string>

//This class is hardcoded to work only with specific button textures
class Button {

    public:
        Button(int hideX, int hideY, int showX, int showY, const std::string& texturePath);

        void update();
        void draw() const;
        void hide();
        void show();
        bool isPressed() const;
        bool isMoving() const;

    private:
        raylib::Texture2D texture;            //button's texture sheet
        raylib::Vector2 hideLocation;         //fixed off-screen location
        raylib::Vector2 showLocation;         //fixed on-screen location
        raylib::Vector2 realLocation;         //button's current location
        raylib::Vector2 location;             //location to be/move to

};

#endif
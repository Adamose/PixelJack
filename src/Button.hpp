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
        void draw();
        void hide();
        void show();
        bool isPressed();
        bool isMoving();

    private:
        raylib::Texture2D texture;            //button's texture sheet
        raylib::Vector2 hideLocation;         //fixed off-screen location
        raylib::Vector2 showLocation;         //fixed on-screen location
        raylib::Vector2 realLocation;         //button's current location
        raylib::Vector2 location;             //location to be/move to

};

#endif
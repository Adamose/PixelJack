#ifndef MENU_H
#define MENU_H

#include "../include/raylib-cpp.hpp"
#include <string>

class Menu {

    public:
        Menu();
        void draw();
        void setShow(bool show);

    private:
        raylib::Texture2D chip;
        raylib::Font font;
        raylib::Rectangle sourceRectangle;
        raylib::Rectangle chipOnePosition;
        raylib::Rectangle chipTwoPosition;
        int timer;
        bool show;

};

#endif
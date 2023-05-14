#include "Menu.hpp"

Menu::Menu() : chip("../resources/images/chips/d_50_chip.png"), timer(0), 
    show(true), font("../resources/misc/PixelCode.otf", 256) {}

void Menu::draw() {

    if (!show) {
        return;
    }

    //Drawing title
    int width = font.MeasureText("PixelJack", 96.0f, 0.0f).GetX();
    font.DrawText("PixelJack", raylib::Vector2(514 - (width / 2), 60), 96.0f, 0.0f);

    //Drawing chips
    chip.Draw(raylib::Rectangle(0, 0, 21, 21), raylib::Rectangle(226, 250, 84, 84), raylib::Vector2(42, 42), timer);
    chip.Draw(raylib::Rectangle(0, 0, 21, 21), raylib::Rectangle(798, 250, 84, 84), raylib::Vector2(42, 42), timer);

    //Rotate chips
    timer += 2;
}

void Menu::setShow(bool show) {
    this->show = show;
}
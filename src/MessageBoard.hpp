#ifndef MESSAGEBOARD_H
#define MESSAGEBOARD_H

#include "../include/raylib-cpp.hpp"
#include <string>

class MessageBoard {

    public:
        MessageBoard();

        void update();
        void draw() const;
        void show();
        void hide();
        void setMessage(int profit);
        void setTitle(int code);
        int getX();

    private:
        float realX;
        int x;
        std::string title;
        std::string message;
        raylib::Texture2D board;
        raylib::Font font;
        const int y;

};

#endif
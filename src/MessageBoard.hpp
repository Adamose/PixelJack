#ifndef MESSAGEBOARD_H
#define MESSAGEBOARD_H

#include "../include/raylib-cpp.hpp"
#include <string>

class MessageBoard {

    public:
        MessageBoard();

        void update();
        void draw();
        void show();
        void hide();
        void setMessage(int profit);
        void setTitle(int code);
        int getX();
        int getTimerWidth();

    private:
        float realX;
        int x;
        std::string title;
        std::string message;
        raylib::Texture2D board;
        raylib::Font font;
        const int y;
        int timerWidth;
        bool showTimer;

};

#endif
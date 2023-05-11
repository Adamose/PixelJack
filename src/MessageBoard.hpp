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
        void setMessage(const std::string& newMessage);

    private:
        float realX;
        int x;
        std::string message;
        raylib::Texture2D board;
        const int y;

};

#endif
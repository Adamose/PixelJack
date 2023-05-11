#include "MessageBoard.hpp"

MessageBoard::MessageBoard() : realX(-300), x(-300), message("Winner"),
    board("../resources/images/Board.png"), y(200) {}

void MessageBoard::update() {

    //Checking if board doesn't need to move
    if (realX == x) {
        return;
    }

    //Checking if board is done moving and needs to be reset to the left
    if (realX == 1100) {
        realX = -300;
        return;
    }

    //Use linear interpolation to move board
    realX = Vector2Lerp(raylib::Vector2(realX, y), raylib::Vector2(x, y), 0.1f).x;
    realX += 1;

    //Checking if board reached it's location
    if (roundf(realX) == x) {
        realX = x;
    }
}

void MessageBoard::draw() const {
    board.Draw(realX, y);
}

void MessageBoard::show() {
    x = 362;
}

void MessageBoard::hide() {
    x = 1100;
}

void MessageBoard::setMessage(const std::string& newMessage) {
    message = newMessage;
}
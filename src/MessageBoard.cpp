#include "MessageBoard.hpp"

MessageBoard::MessageBoard() : realX(-300), x(-300), title("title"),
    message("Click To Start"), board("../resources/images/Board.png"), font("../resources/misc/PixelCode.otf", 256), y(200) {}

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

    int width = font.MeasureText(title, 32.0f, 0.0f).GetX();
    font.DrawText(title, raylib::Vector2(realX + 150 - (width / 2), 220), 32.0f, 0.0f);

    width = font.MeasureText(message, 24.0f, 0.0f).GetX();
    font.DrawText(message, raylib::Vector2(realX + 150 - (width / 2), 255), 24.0f, 0.0f);
}

void MessageBoard::show() {
    x = 362;
}

void MessageBoard::hide() {
    x = 1100;
}

void MessageBoard::setMessage(int profit) {
   if (profit < 0) {
        message = "You lost $" + std::to_string(profit * -1);
   } else {
        message = "You made $" + std::to_string(profit);
   }
}

void MessageBoard::setTitle(int code) {
    switch (code) {

        case 0:
            title = "PLAY";
            break;

        case 1:
            title = "WIN";
            break;

        case 2:
            title = "LOSE";
            break;

        case 3:
            title = "TIE";
            break;

        case 4:
            title = "BLACKJACK";

        case 5:
            title = "WIN/WIN";
            break;

        case 6:
            title = "LOSE/LOSE";
            break;

        case 7:
            title = "WIN/LOSE";
            break;

        case 8:
            title = "WIN/TIE";
            break;

        case 9:
            title = "LOSE/TIE";
            break;

        case 10:
            title = "TIE/TIE";
            break;

        case 11:
            title = "BLACKJACK/WIN";
            break;
        
        case 12:
            title = "BLACKJACK/LOSE";
            break;

        case 13:
            title = "BLACKJACK/TIE";
            break;

        case 14:
            title = "BLACKJAKC/BLACKJACK";
    }
}
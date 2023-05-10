#include "Card.hpp"

Card::Card(int id, int x, int y, raylib::Texture2D** textures)
 : id(id), value(std::min(ceil(((float) id + 1) / 4), 10.0f)), realLocation(1100, -100), 
   location(x, y), textures(textures), facedown(false) {}

//Method to animate card's movement
void Card::update() {

    //Checking if card doesn't need to move
    if (realLocation == location) {
        return;
    }

    float speed = 0.15;

    //Check if card is being discarded
    if (location.x == -141) {
        speed = 0.07;
    }

    //Use linear interpolation to move card
    realLocation = Vector2Lerp(realLocation, location, speed);

    //Checking if card reached it's location (check if rounded reallocation is same as location)
    if (raylib::Vector2(roundf(realLocation.x), roundf(realLocation.y)) == location) {
        realLocation = location;
    }
}

//Method to draw card texture
void Card::draw() const {

    //Check if card is face down
    if (facedown) {
        textures[52]->Draw(realLocation, 0.0f, 2.0f);
    } else {
        textures[id]->Draw(realLocation, 0.0f, 2.0f);
    }
}

//Method to set card's new location to move to
void Card::setLocation(int x, int y) {
    location.x = x;
    location.y = y;
}

//Method to check if card is currently moving
bool Card::isMoving() {
    return realLocation != location;
}

int Card::getValue() const {
    return value;
}

void Card::setFacedown(bool boolean) {
    facedown = boolean;
}

bool Card::getFacedown() const {
    return facedown;
}
#include "Card.hpp"

Card::Card(int id, int x, int y, raylib::Texture2D** textures)
 : id(id), value(std::min(ceil(((float) id + 1) / 4), 10.0f)), realLocation(1100, -100), location(x, y), textures(textures) {}

//Method to animate card's movement
void Card::update() {

    //Checking if card doesn't need to move
    if (realLocation == location) {
        return;
    }

    //Use linear interpolation to move card
    realLocation = Vector2Lerp(realLocation, location, 0.15f);

    //Checking if card reached it's location (check if rounded reallocation is same as location)
    if (raylib::Vector2(roundf(realLocation.x), roundf(realLocation.y)) == location) {
        realLocation = location;
    }
}

//Method to draw card texture
void Card::draw() const {

    //Drawing texture with 0 rotation and 2 times scaling
    textures[id]->Draw(realLocation, 0.0f, 2.0f);
}

//Method to set card's new location to move to
void Card::setLocation(int x, int y) {
    location.x = x;
    location.y = y;
}

//Method to check if card is currently moving
bool Card::isMoving() const {
    return realLocation != location;
}

int Card::getValue() const {
    return value;
}
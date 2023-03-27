#include "Card.hpp"

Card::Card(int id, int x, int y, raylib::Texture2D** textures)
 : id(id), value(id), realLocation(x, y), textures(textures) {}

//Method to animate card's movement
void Card::update() {

    //Checking if card needs to move
    if (realLocation != location) {

        //Use linear interpolation to move card
        realLocation = Vector2Lerp(realLocation, location, 0.15f);

        //Checking if card reached it's location
        if (realLocation.Equals(location)) {
            realLocation = location;
        }
    }
}

//Method to draw card texture
void Card::draw() const {

    //Drawing texture with 0 rotation and 2 times scaling
    textures[id]->Draw(realLocation, 0.0f, 2.0f);
}

void Card::setX(int x) {
    location.x = x;
}

void Card::setY(int y) {
    location.y = y;
}
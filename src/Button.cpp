#include "Button.hpp"

Button::Button(int hideX, int hideY, int showX, int showY, const std::string& texturePath)
 : hideLocation(hideX, hideY), showLocation(showX, showY), texture(texturePath),
   realLocation(hideLocation), location(realLocation) {}

//Method to animate button's movement
void Button::update() {

    //Checking if button needs to move
    if (realLocation != location) {
        realLocation = Vector2MoveTowards(realLocation, location, 5.0f);
    }
}

//Draw button on the screen
void Button::draw() const {
    
    //Checking if button is clickable (at showLocation)
    if (realLocation != showLocation) {
        texture.Draw(raylib::Rectangle(0, 0, 30, 15), raylib::Rectangle(realLocation.x, realLocation.y, 90, 45), raylib::Vector2(0, 0), 0.0f, LIGHTGRAY);
        return;
    }

    //Checking if mouse is hovering over button
    if (CheckCollisionPointRec(GetMousePosition(), raylib::Rectangle(realLocation.x, realLocation.y, 90, 45))) {

        //Check if mouse left button is down on button
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {

            //Drawing shorter button texture to give illusion of button being pressed
            texture.Draw(raylib::Rectangle(0, 15, 30, 15), raylib::Rectangle(realLocation.x, realLocation.y, 90, 45), raylib::Vector2(0, 0), 0.0f);
            return;
        }

        //Drawing brighter button to show it's clickable
        texture.Draw(raylib::Rectangle(0, 0, 30, 15), raylib::Rectangle(realLocation.x, realLocation.y, 90, 45), raylib::Vector2(0, 0), 0.0f);
        return;   
    }

    texture.Draw(raylib::Rectangle(0, 0, 30, 15), raylib::Rectangle(realLocation.x, realLocation.y, 90, 45), raylib::Vector2(0, 0), 0.0f, LIGHTGRAY);
}

//Checks if button was pressed
bool Button::isPressed() const {

    //Checking if mouse left button was released
    if (!IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        return false;
    }
   
    //Checking if button is visible on screen
    if (realLocation != showLocation) {
        return false;
    }

    //Return the resut of collision check
    return CheckCollisionPointRec(GetMousePosition(), raylib::Rectangle(realLocation.x, realLocation.y, 90, 45));
}

//Sets button location to be the hide location, this moves the button off the screen
void Button::hide() {
    location = hideLocation;
}

//Sets button location to be the show location, this moves the button onto the screen
void Button::show() {
    location = showLocation;
}

//Method to check if button is currently moving
bool Button::isMoving() const {
    return realLocation != location;
}

bool Button::isActive() const {
    return active;
}

void Button::setActive(bool boolean) {
    active = boolean;
}
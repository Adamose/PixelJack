#include "../include/raylib-cpp.hpp"
#include "GamePanel.hpp"

#ifndef PLATFORM_WEB

//Entry point of program
int main() {

    //Creating window instance
    raylib::Window window(1024, 500);
    
    //Creating audio device (constructor initializes it and destructor closes it)
    raylib::AudioDevice audioDevice;
    
    //Setting window's properties
    window.SetTargetFPS(30);
    window.SetIcon(raylib::Image("../resources/misc/PixelJack.png"));
    window.SetTitle("Pixel Jack");

    //Creating GamePanel instance
    GamePanel panel;

    //Game loop running at 30hz
    while (!window.ShouldClose()) {
        window.BeginDrawing();

        //Drawing frame
        panel.draw();

        window.EndDrawing();
    }

    return 0;
}

#else

#include <emscripten/emscripten.h>

raylib::Window* windowPointer;
GamePanel* panelPointer;

void drawFrame() {
        windowPointer->BeginDrawing();
        panelPointer->draw();
        windowPointer->EndDrawing();
}

//Entry point of program
int main() {

    //Creating window instance
    raylib::Window window(1024, 500);
    windowPointer = &window;

    //Creating audio device (constructor initializes it and destructor closes it)
    raylib::AudioDevice audioDevice;
    
    //Creating GamePanel instance
    GamePanel panel;
    panelPointer = &panel;

    //Game loop running at 30hz
    emscripten_set_main_loop(drawFrame, 30, 1);
    
    return 0;
}

#endif
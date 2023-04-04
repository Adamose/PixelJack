#include "raylib-cpp.hpp"
#include "GamePanel.hpp"

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
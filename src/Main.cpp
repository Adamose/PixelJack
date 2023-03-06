#include "raylib-cpp.hpp"
#include "GamePanel.hpp"

//Entry point of program
int main() {

    //Creating window instance
    raylib::Window window(1280, 720);
    
    //Setting window's properties
    window.SetTargetFPS(30);
    window.SetIcon(raylib::Image("../resources/misc/PixelJack.png"));
    window.SetTitle("Pixel Jack");

    //Creating GamePanel instance
    GamePanel panel(window.GetWidth(), window.GetHeight());

    //Game loop running at 30hz
    while (!window.ShouldClose()) {
        window.BeginDrawing();

        //Updating and drawing frame
        panel.update();
        panel.draw();

        window.EndDrawing();
    }

    return 0;
}
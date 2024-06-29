#include <iostream>
#include "includes/gui.h"
#include <raylib.h>
#include "framerateManager.h"

constexpr int screenWidth = 800;
constexpr int screenHeight = 800;

int main() {
    InitWindow(screenWidth, screenHeight, "Setup Controller");
    Gui::Instantiate();
    FramerateManager framerateManager;
    Game game{};


    while (!WindowShouldClose()) {
        game.zaber.Update();
        Gui::Update(game);
        BeginDrawing();
        ClearBackground(DARKGRAY);
        Gui::Draw();
//        DrawFPS(10, 10);
        EndDrawing();
        framerateManager.Update();
    }

    CloseWindow();
}


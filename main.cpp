#include "includes/gui.h"
#include <raylib.h>
#include "framerateManager.h"

import game;

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 800, "Setup Control");
    const auto image = LoadImage("rat_icon2.png");
    SetWindowIcon(image);

    Gui::Instantiate();
    FramerateManager framerateManager;
    Game game{};


    while (!WindowShouldClose()) {
        game.update();
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


#include <iostream>
#include "includes/gui.h"
#include "includes/metadata.h"
#include "framerateManager.h"
#include <raylib.h>
#include <algorithm>

const int screenWidth = 800;
const int screenHeight = 800;

int main() {
    InitWindow(screenWidth, screenHeight, "Setup Controller");
    Metadata metadata = Metadata{17};
    Gui::Instantiate();
    FramerateManager framerateManager;


    while (!WindowShouldClose()) {
        Gui::Update(metadata.getRootNode());
        BeginDrawing();
        ClearBackground(DARKGRAY);
        Gui::Draw();
        DrawFPS(10, 10);
        EndDrawing();
        framerateManager.Update();
    }

    CloseWindow();
}


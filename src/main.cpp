#include <raylib.h>

#include "world.hpp"

int main() {
    InitWindow(800, 450, "Grid Pathfinding");
    SetTargetFPS(60);

    demo::world world;

    world.start();

    while (!WindowShouldClose()) {
        world.update(GetFrameTime());

        BeginDrawing();
        ClearBackground(RAYWHITE);

        world.draw();

        EndDrawing();
    }

    CloseWindow();
}
#include <raylib.h>

#include "world.hpp"
#include "grid/world_grid.hpp"

#include "ecs/entity/main_camera.hpp"
#include "ecs/entity/zombie.hpp"

#include "ecs/systems/auto_nav_move.hpp"
#include "ecs/systems/auto_nav_requester.hpp"
#include "ecs/systems/camera_movement.hpp"
#include "ecs/systems/test_grid_selector.hpp"

#include "resource/test_assets.hpp"

int main() {
    InitWindow(800, 450, "Grid Pathfinding");
    SetTargetFPS(60);

    demo::world world;
    demo::world_grid world_grid;

    demo::test_assets test_assets;

    // add context
    world.add_context<demo::world_grid>();

    // add systems
    world.add_system<demo::ecs::camera_movement>();
    world.add_system<demo::ecs::test_grid_selector>();
    world.add_system<demo::ecs::auto_nav_requester>();
    world.add_system<demo::ecs::auto_nav_move>();

    // register hooks
    world.on_hooks();

    // add entities
    world.add_entity<demo::ecs::main_camera>();
    world.add_entity<demo::ecs::zombie>(test_assets);

    world.start();

    while (!WindowShouldClose()) {
        world.update(GetFrameTime());

        BeginDrawing();
        ClearBackground(RAYWHITE);

        world.draw();

        EndDrawing();
    }

    world.end();

    CloseWindow();
}
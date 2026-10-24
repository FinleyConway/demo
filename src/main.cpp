#include <raylib.h>

#include "world.hpp"
#include "grid/world_grid.hpp"

#include "ecs/entity/main_camera.hpp"
#include "ecs/entity/pillar.hpp"

#include "ecs/systems/navigation/auto_nav_move.hpp"
#include "ecs/systems/navigation/auto_nav_requester.hpp"
#include "ecs/systems/camera_movement.hpp"
#include "ecs/systems/test_grid_selector.hpp"
#include "ecs/systems/place_pillar.hpp"

#include "utils/asset_context.hpp"

int main() {
    InitWindow(800, 450, "Grid Pathfinding");
    SetTargetFPS(60);

    demo::world world;

    // add context
    world.add_context<demo::world_grid>();
    world.add_context<demo::asset_context>();

    // add systems
    world.add_system<demo::ecs::camera_movement>();
    world.add_system<demo::ecs::test_grid_selector>();
    world.add_system<demo::ecs::auto_nav_requester>();
    world.add_system<demo::ecs::auto_nav_move>();
    world.add_system<demo::ecs::place_pillar>();

    // register hooks
    world.on_hooks();

    // add entities
    world.add_entity<demo::ecs::main_camera>();
    world.add_entity<demo::ecs::pillar>();

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
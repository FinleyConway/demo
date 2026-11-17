#include <raylib.h>

#include "ecs/world.hpp"
#include "world/path_finding/path_finder.hpp"

#include "ecs/features/camera/camera.hpp"

using namespace demo;

int main() {
    ecs::world world;

    world.register_features<
        ecs::camera_future
    >();

    // demo::ecs::positioning_feature::register_to(world); // internal, dont really like this feature systems
    // demo::ecs::ui_feature::register_input_to(world);

    // demo::ecs::camera_feature::register_to(world);
    // demo::ecs::combat_feature::register_to(world);
    // demo::ecs::health_feature::register_to(world);
    // demo::ecs::map_feature::register_to(world);
    // demo::ecs::navigation_feature::register_to(world);
    // demo::ecs::settlement_feature::register_to(world);
    
    // // internal
    // demo::ecs::ui_feature::register_to(world);
    // demo::ecs::rendering_feature::register_to(world); 

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "Grid Pathfinding");
    SetTargetFPS(60);

    // world.load(
    //     "/var/home/finley/Projects/Git/demo/assets/assets", 
    //     "/var/home/finley/Projects/Git/demo/assets/prefabs", 
    //     "/var/home/finley/Projects/Git/demo/assets/entities"
    // );

    world.add_context<world_grid>();
    world.add_context<path_finder>();

    world.on_hooks();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        world.update(GetFrameTime());

        EndDrawing();
    }

    CloseWindow();
}
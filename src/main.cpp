#include <raylib.h>

#include "ecs/features/spawner/spawner.hpp"
#include "ecs/world.hpp"
#include "world/path_finding/path_finder.hpp"

#include "ecs/features/positioning/positioning.hpp"
#include "ecs/features/camera/camera.hpp"
#include "ecs/features/combat/combat.hpp"
#include "ecs/features/health/health.hpp"
#include "ecs/features/map/map.hpp"
#include "ecs/features/rendering/rendering.hpp"
#include "ecs/features/settlement_builder/settlement_builder.hpp"
#include "ecs/features/interactable/interactable.hpp"
#include "ecs/features/ui/ui.hpp"

using namespace demo;

int main() {
    ecs::world world;

    world.register_features<
        ecs::positioning_feature,

        ecs::camera_future,
        ecs::spawner_feature,
        ecs::interactable_feature,
        ecs::settlement_feature,
        ecs::map_feature,
        ecs::combat_future,
        ecs::health_feature,
        //ecs::navigation_future,
        
        ecs::ui_feature,
        ecs::rendering_feature
    >(); 

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "Grid Pathfinding");
    SetTargetFPS(60);

    world.load(
        "/var/home/finley/Projects/Git/demo/assets/assets", 
        "/var/home/finley/Projects/Git/demo/assets/prefabs", 
        "/var/home/finley/Projects/Git/demo/assets/entities"
    );

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
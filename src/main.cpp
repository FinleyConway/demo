#include <raylib.h>

#include "ecs/features/camera/camera.hpp"
#include "ecs/features/combat/combat.hpp"
#include "ecs/features/health/health.hpp"
#include "ecs/features/map/map.hpp"
#include "ecs/features/navigation/navigation.hpp"
#include "ecs/features/positioning/positioning.hpp"
#include "ecs/features/rendering/rendering.hpp"
#include "ecs/features/settlement_builder/settlement_builder.hpp"

#include "ecs/features/ui/ui.hpp"
#include "world/world.hpp"

int main() {
    demo::world world;

    demo::ecs::positioning_feature::register_to(world); // internal, dont really like this feature systems
    demo::ecs::ui_feature::register_input_to(world);

    demo::ecs::camera_feature::register_to(world);
    demo::ecs::combat_feature::register_to(world);
    demo::ecs::health_feature::register_to(world);
    demo::ecs::map_feature::register_to(world);
    demo::ecs::navigation_feature::register_to(world);
    demo::ecs::settlement_feature::register_to(world);
    
    // internal
    demo::ecs::ui_feature::register_to(world);
    demo::ecs::rendering_feature::register_to(world); 

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "Grid Pathfinding");
    SetTargetFPS(60);

    world.load(
        "/var/home/finley/Projects/Git/demo/assets/assets", 
        "/var/home/finley/Projects/Git/demo/assets/prefabs", 
        "/var/home/finley/Projects/Git/demo/assets/entities"
    );

    world.add_context<demo::world_grid>();
    world.add_context<demo::path_finder>();

    world.on_hooks();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        world.update(GetFrameTime());

        EndDrawing();
    }

    CloseWindow();
}
#include <raylib.h>

#include "ecs/systems/camera/camera_movement.hpp"

#include "ecs/systems/combat/generate_attack_area.hpp"
#include "ecs/systems/combat/turret_instant_attack.hpp"
#include "ecs/systems/combat/turret_searching.hpp"

#include "ecs/systems/health/entity_killed.hpp"
#include "ecs/systems/health/health_addition.hpp"
#include "ecs/systems/health/health_reduction.hpp"

#include "ecs/systems/navigation/auto_nav_move.hpp"
#include "ecs/systems/navigation/auto_nav_requester.hpp"

#include "ecs/systems/placement/place_pillar.hpp"
#include "ecs/systems/placement/grid_entity_placer.hpp"
#include "ecs/systems/placement/test_grid_selector.hpp"

#include "world/world.hpp"
#include "world/world_grid/world_grid.hpp"

#include "ecs/prefabs/camera/main_camera.hpp"
#include "ecs/prefabs/defence/pillar.hpp"

#include "resources/asset_context.hpp"

int main() {
    InitWindow(800, 450, "Grid Pathfinding");
    SetTargetFPS(60);

    demo::world world;

    // add context
    world.add_context<demo::world_grid>();
    world.add_context<demo::asset_context>();

    // add systems
    world.add_system<demo::ecs::camera_movement>();

    world.add_system<demo::ecs::generate_attack_area>();
    world.add_system<demo::ecs::turret_searching>();
    world.add_system<demo::ecs::turret_instant_attack>();

    world.add_system<demo::ecs::grid_entity_placer>();
    world.add_system<demo::ecs::test_grid_selector>();

    world.add_system<demo::ecs::auto_nav_requester>();
    world.add_system<demo::ecs::auto_nav_move>();

    world.add_system<demo::ecs::health_addition>();
    world.add_system<demo::ecs::health_reduction>();
    world.add_system<demo::ecs::entity_killed>();

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
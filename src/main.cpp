#include <raylib.h>

#include "ecs/entities/map_controller.hpp"
#include "ecs/systems/camera/camera_movement.hpp"

#include "ecs/systems/combat/defence/generate_attack_area.hpp"
#include "ecs/systems/combat/defence/turret_instant_attack.hpp"
#include "ecs/systems/combat/defence/turret_searching.hpp"

#include "ecs/systems/combat/enemies/obstruction_clearance.hpp"

#include "ecs/systems/health/entity_killed.hpp"
#include "ecs/systems/health/health_addition.hpp"
#include "ecs/systems/health/health_reduction.hpp"

#include "ecs/systems/navigation/navigation_request.hpp"
#include "ecs/systems/navigation/navigation_results.hpp"
#include "ecs/systems/navigation//navigation_move.hpp"

#include "ecs/systems/placement/place_pillar.hpp"
#include "ecs/systems/placement/grid_entity_placer.hpp"
#include "ecs/systems/placement/test_grid_selector.hpp"

#include "ecs/entities/camera/main_camera.hpp"

#include "ecs/prefabs/defence/pillar.hpp"
#include "ecs/prefabs/enemies/zombie.hpp"
#include "ecs/prefabs/defence/wall.hpp"

#include "ecs/systems/world_grid_setup.hpp"
#include "world/world.hpp"
#include "world/world_grid/world_grid.hpp"
#include "world/path_finding/path_finder.hpp"


int main() {
    InitWindow(800, 450, "Grid Pathfinding");
    SetTargetFPS(60);

    demo::world world;

    // add context
    world.add_context<demo::world_grid>();
    world.add_context<demo::path_finder>();

    // create prefabs
    auto test_prefab = world.emplace_prefab<demo::ecs::zombie>();
    auto wall_prefab = world.emplace_prefab<demo::ecs::wall>();

    // add systems
    world.add_system<demo::ecs::camera_movement>();

    world.add_system<demo::ecs::generate_attack_range>();
    world.add_system<demo::ecs::attack_area_search>();
    world.add_system<demo::ecs::turret_instant_attack>();

    world.add_system<demo::ecs::world_grid_setup>();
    world.add_system<demo::ecs::grid_entity_placer>();
    world.add_system<demo::ecs::test_grid_selector>();

    world.add_system<demo::ecs::navigation_request>();
    world.add_system<demo::ecs::navigation_results>();
    world.add_system<demo::ecs::navigation_move>();

    world.add_system<demo::ecs::obstruction_clearance>();

    world.add_system<demo::ecs::health_addition>();
    world.add_system<demo::ecs::health_reduction>();
    world.add_system<demo::ecs::entity_killed>();

    world.add_system<demo::ecs::place_pillar>();

    // register hooks
    world.on_hooks();

    // add entities
    world.add_entity<demo::ecs::main_camera>();
    world.add_entity<demo::ecs::pillar>();

    world.add_entity<demo::ecs::map_controller>(wall_prefab);

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
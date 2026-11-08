#include <raylib.h>

#include "world/world.hpp"
#include "world/world_grid/world_grid.hpp"
#include "world/path_finding/path_finder.hpp"

#include "ecs/prefabs/defence/wall.hpp"
#include "ecs/prefabs/defence/test_turret.hpp"

#include "ecs/entities/ui/button.hpp"
#include "ecs/prefabs/defence/core.hpp"
#include "ecs/entities/map_controller.hpp"
#include "ecs/entities/camera/main_camera.hpp"

#include "ecs/features/ui/ui.hpp"
#include "ecs/features/map/map_setup.hpp"
#include "ecs/features/camera/camera_setup.hpp"
#include "ecs/features/combat/combat_setup.hpp"
#include "ecs/features/health/health_setup.hpp"
#include "ecs/features/rendering/rendering_setup.hpp"
#include "ecs/features/navigation/navigation_setup.hpp"
#include "ecs/features/positioning/positioning_setup.hpp"
#include "ecs/features/settlement_builder/settlement_builder_setup.hpp"

using namespace demo;

void setup_systems(world& world) {
    ecs::positioning_feature::register_systems(world);

    ecs::ui_feature::register_systems(world);

    ecs::camera_feature::register_systems(world);

    ecs::navigation_feature::register_systems(world);
    ecs::combat_feature::register_systems(world);
    ecs::health_feature::register_systems(world);

    ecs::map_feature::register_systems(world);
    ecs::settlement_feature::register_systems(world);

    ecs::rendering_feature::register_systems(world);
}

void setup_entities(world& world) {
    auto wall_prefab = world.emplace_prefab<ecs::wall>();
    auto turret_prefab = world.emplace_prefab<ecs::test_turret>();

    world.add_entity<ecs::main_camera>();
    world.add_entity<ecs::map_controller>(
        wall_prefab,
        turret_prefab
    );
    world.add_entity<ecs::core>();
    world.add_entity<ecs::button>();
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "Grid Pathfinding");
    SetTargetFPS(60);

    world world;

    // add context
    world.add_context<world_grid>();
    world.add_context<path_finder>();

    // add systems
    setup_systems(world);

    // register hooks
    world.on_hooks();

    // create prefabs and add entities
    setup_entities(world);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        world.update(GetFrameTime());

        EndDrawing();
    }

    CloseWindow();
}
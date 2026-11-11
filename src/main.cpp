#include "ecs/features/camera/camera.hpp"
#include "ecs/features/combat/combat.hpp"
#include "ecs/features/health/health.hpp"
#include "ecs/features/map/map.hpp"
#include "ecs/features/navigation/navigation.hpp"
#include "ecs/features/positioning/positioning.hpp"
#include "ecs/features/settlement_builder/settlement_builder.hpp"

#include "world/world.hpp"

int main() {
    demo::world world;

    demo::ecs::camera_feature::register_to(world);
    demo::ecs::combat_feature::register_to(world);
    demo::ecs::health_feature::register_to(world);
    demo::ecs::map_feature::register_to(world);
    demo::ecs::navigation_feature::register_to(world);
    demo::ecs::positioning_feature::register_to(world);
    demo::ecs::settlement_feature::register_to(world);

    world.load(
        "/var/home/finley/Projects/Git/demo/test_assets/prefabs/", 
        "/var/home/finley/Projects/Git/demo/test_assets/entities/"
    );
}
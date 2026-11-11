#include <iostream>

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "ecs/core/prefab_manager.hpp"
#include "ecs/features/settlement_builder/components/structures.hpp"
#include "resources/asset_context.hpp"
#include "ecs/core/entity_loader.hpp"

#include "ecs/features/camera/camera.hpp"
#include "ecs/features/combat/combat.hpp"
#include "ecs/features/health/health.hpp"
#include "ecs/features/map/map.hpp"
#include "ecs/features/navigation/navigation.hpp"
#include "ecs/features/positioning/positioning.hpp"
#include "ecs/features/settlement_builder/settlement_builder.hpp"

int main() {
    entt::registry registry;
    demo::asset_context context;

    demo::ecs::prefab_manager pre(registry);
    demo::ecs::entity_loader reg(registry, pre, context);

    demo::ecs::camera_feature::register_components(reg);
    demo::ecs::combat_feature::register_components(reg);
    demo::ecs::health_feature::register_components(reg);
    demo::ecs::map_feature::register_components(reg);
    demo::ecs::navigation_feature::register_components(reg);
    demo::ecs::positioning_feature::register_components(reg);
    demo::ecs::settlement_feature::register_components(reg);

    reg.load(
        "/var/home/finley/Projects/Git/demo/test_assets/prefabs/", 
        "/var/home/finley/Projects/Git/demo/test_assets/entities/"
    );
}
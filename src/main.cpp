#include <iostream>

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "ecs/core/prefab_manager.hpp"
#include "ecs/features/camera/components/camera_bounds.hpp"
#include "ecs/features/camera/components/camera_drag.hpp"
#include "ecs/features/camera/components/camera_zoom.hpp"
#include "resources/asset_context.hpp"
#include "ecs/core/entity_loader.hpp"

#include "ecs/features/camera/camera.hpp"

int main() {
    entt::registry registry;
    demo::asset_context context;

    demo::ecs::prefab_manager pre(registry);
    demo::ecs::entity_loader reg(registry, pre, context);

    demo::ecs::camera_feature::regiseter_components(reg);

    auto entity = reg.entity_from_file("/var/home/finley/Projects/Git/demo/test_assets/entities/camera.json");

    std::cout << registry.all_of<
        demo::ecs::camera_bounds_init,
        demo::ecs::camera_bounds,
        demo::ecs::camera_drag
    >(entity) << std::endl;

    auto& zoom = registry.get<demo::ecs::camera_zoom>(entity);

    std::cout << zoom.increments << std::endl;
    std::cout << zoom.min << std::endl;
    std::cout << zoom.max << std::endl;
}
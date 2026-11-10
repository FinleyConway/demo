#include <iostream>

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "ecs/core/prefab_manager.hpp"
#include "resources/asset_context.hpp"
#include "ecs/core/entity_loader.hpp"

struct position {
    float x = 0;
    float y = 0;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(position, x, y);
};

struct rotation {
    float angle = 0;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(rotation, angle);
};

struct scale {
    float x = 0;
    float y = 0;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(scale, x, y);
};

int main() {
    entt::registry registry;
    demo::asset_context context;

    demo::ecs::prefab_manager pre(registry);
    demo::ecs::entity_loader reg(registry, pre, context);

    reg.register_component<position>();
    reg.register_component<rotation>();
    reg.register_component<scale>();

    auto prefab = reg.prefab_from_file("/var/home/finley/Projects/Git/demo/test_assets/entities/test_entity.json");

    auto e = pre.instantiate(prefab);

    auto& p = registry.get<position>(e);
    auto& r = registry.get<rotation>(e);
    auto& s = registry.get<scale>(e);

    std::cout << p.x << " " << p.y << std::endl;
    std::cout << r.angle << std::endl;
    std::cout << s.x << " " << s.y << std::endl;
}
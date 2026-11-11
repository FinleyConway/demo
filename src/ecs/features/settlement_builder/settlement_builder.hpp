#pragma once

#include "ecs/features/settlement_builder/components/settlement_core.hpp"
#include "ecs/features/settlement_builder/components/settlement_zone.hpp"
#include "ecs/features/settlement_builder/components/structure_size.hpp"
#include "ecs/features/settlement_builder/components/structures.hpp"

#include "ecs/features/settlement_builder/systems/place_settlement_core.hpp"
#include "ecs/features/settlement_builder/systems/place_structures.hpp"

#include "world/world.hpp"

namespace demo::ecs {
    struct settlement_feature {
        static void register_to(world& world) {
            world.register_component<structures>([](const nlohmann::json& j, const asset_context&, prefab_finder& p) {
                return structures {
                    .wall_prefab = p.get(j["wall_prefab"].get<std::string>()),
                    .turret_prefab = p.get(j["turret_prefab"].get<std::string>()),
                };
            });
            world.register_component<settlement_zone>();
            world.register_component<settlement_core_init>();
            world.register_component<settlement_core>();
            world.register_component<structure_size>();


            world.register_system<place_settlement_core>();
            world.register_system<place_structures>();
        }
    };
}
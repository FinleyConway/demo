#pragma once

#include "ecs/features/settlement_builder/components/settlement_core.hpp"
#include "ecs/features/settlement_builder/components/settlement_zone.hpp"
#include "ecs/features/settlement_builder/components/structure_size.hpp"
#include "ecs/features/settlement_builder/components/structures.hpp"

#include "ecs/features/settlement_builder/systems/place_settlement_core.hpp"
#include "ecs/features/settlement_builder/systems/place_structures.hpp"

#include "world/world.hpp"
#include "ecs/core/entity_loader.hpp"

namespace demo::ecs {
    struct settlement_feature {
        static void register_components(entity_loader& loader) {
            loader.register_component<structures>([](const nlohmann::json& j, const asset_context&, prefab_finder& p) {
                return structures {
                    .wall_prefab = p.get(j["wall_prefab"].get<std::string>()),
                    .turret_prefab = p.get(j["turret_prefab"].get<std::string>()),
                };
            });
            
            loader.register_component<settlement_zone>();
            loader.register_component<settlement_core_init>();
            loader.register_component<settlement_core>();
            loader.register_component<structure_size>();
        }

        static void register_systems(world& world) {
            world.add_system<place_settlement_core>();
            world.add_system<place_structures>();
        }
    };
}
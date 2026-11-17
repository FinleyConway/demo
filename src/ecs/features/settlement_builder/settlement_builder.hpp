#pragma once

#include "ecs/features/settlement_builder/components/builder_state.hpp"
#include "ecs/features/settlement_builder/components/settlement_core.hpp"
#include "ecs/features/settlement_builder/components/settlement_zone.hpp"
#include "ecs/features/settlement_builder/components/structure_size.hpp"
#include "ecs/features/settlement_builder/components/structures.hpp"

#include "ecs/features/settlement_builder/systems/place_structures.hpp"

#include "ecs/world.hpp"

namespace demo::ecs {
    struct settlement_feature {
        using components = entt::type_list<
            settlement_zone,
            settlement_core,
            structure_size,
            builder_state
        >;

        //using events = entt::type_list<>;
        
        using systems = entt::type_list<
            place_structures
        >;

        static void configure(world& world) {
            world.register_component<structures>([](const nlohmann::json& j, const asset_context&, prefab_finder& p) {
                return structures {
                    .core_prefab = p.get(j["core_prefab"].get<std::string>()),
                    .wall_prefab = p.get(j["wall_prefab"].get<std::string>()),
                    .turret_prefab = p.get(j["turret_prefab"].get<std::string>()),
                };
            });
        }
    };
}
#pragma once

#include "world/world.hpp"
#include "ecs/features/settlement_builder/settlement_builder.hpp"

namespace demo::ecs {
    struct settlement_map_spec {
        prefab_id wall_prefab;
        prefab_id turret_prefab;
    };

    struct settlement_structure_spec {
        bool is_core = false;
        cell size;
    };

    struct settlement_feature {
        static void make_map(entt::registry& registry, entt::entity entity, const settlement_map_spec& spec) {
            registry.emplace<structures>(entity, structures {
                .wall_prefab = spec.wall_prefab,
                .turret_prefab = spec.turret_prefab,
            });
            registry.emplace<settlement_zone>(entity);
        }

        static void make_structure(entt::registry& registry, entt::entity entity, const settlement_structure_spec& spec) {
            if (spec.is_core) {
                 registry.emplace<settlement_core_init>(entity);
                 registry.emplace<settlement_core>(entity);
            }
            
            registry.emplace<structure_size>(entity, structure_size {
                .value = spec.size
            });
        }

        static void register_systems(world& world) {
            world.add_system<place_settlement_core>();
            world.add_system<place_structures>();
        }
    };
}
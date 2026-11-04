#pragma once

#include "world/world.hpp"
#include "ecs/features/positioning/positioning.hpp"

namespace demo::ecs {
    struct positioning_feature {
        struct positioning_spec {
            bool is_static = false;
            point position;
        };

        static void make_positioning(entt::registry& registry, entt::entity entity, const positioning_spec& spec) {
            if (spec.is_static) {
                registry.emplace<static_entity>(entity);
            }
            
            registry.emplace<position>(entity, position {
                .value = spec.position
            });
        }

        static void register_systems(world& world) {
            world.add_system<apply_position>();
        }
    };
}
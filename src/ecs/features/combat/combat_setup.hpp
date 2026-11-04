#pragma once

#include "world/world.hpp"
#include "ecs/features/combat/combat.hpp"

namespace demo::ecs {
    struct combat_spec {
        attack_pattern::shape attack_shape = attack_pattern::shape::none;
        float radius = 0.0f;
        uint16_t angle = 0; // sector direction in degrees
        uint16_t width = 0; // sector width in degrees
        float attack_cooldown = 0.0f;
        attack_preference::type attack_pref = attack_preference::type::enemies;
    };

    struct combat_feature {
        static void register_systems(world& world) {
            world.add_system<generate_attack_area>();

            // init attacks
            world.add_system<acquire_attack_target>();
            world.add_system<attack_obstruction>();

            // attack cooldowns
            world.add_system<execute_attack>();

            // attack types
            world.add_system<perform_instant_attack>();
        }

        static void make_combat(entt::registry& registry, entt::entity entity, const combat_spec& spec) {
            if (spec.attack_shape != attack_pattern::shape::none) {
                registry.emplace<attack_area_init>(entity);
                registry.emplace<attack_area>(entity);

                registry.emplace<attack_pattern>(entity, attack_pattern {
                    .attack_shape = spec.attack_shape,
                    .radius = spec.radius,
                    .angle = spec.angle,
                    .width = spec.width,
                });
            }

            registry.emplace<attack_cooldown>(entity, attack_cooldown {
                .duration = spec.attack_cooldown
            });

            registry.emplace<attack_preference>(entity, attack_preference {
                .value = spec.attack_pref,
            });
        }
    };
}
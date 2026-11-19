#pragma once

#include <entt/entt.hpp>

#include "ecs/features/spawner/components/enemies.hpp"
#include "ecs/features/spawner/systems/horde_spawner.hpp"
#include "ecs/world.hpp"

namespace demo::ecs {
    struct spawner_feature {
        //using components = entt::type_list<>;

        //using events = entt::type_list<>;

        using systems = entt::type_list<
            horde_spawner
        >;

        static void configure(world& world) {
            world.register_component<enemies>(
                [](const nlohmann::json& json, const asset_context& context, prefab_finder& finder) {
                    enemies e;

                    e.horder = finder.get(json["horder"]);

                    return e;
                }
            );
        }
    };
}
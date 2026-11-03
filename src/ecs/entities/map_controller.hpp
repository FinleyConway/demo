#pragma once

#include "ecs/core/entity_builder.hpp"

#include "ecs/components/rendering.hpp"
#include "ecs/components/positional.hpp"
#include "ecs/components/base_management.hpp"

#include "ecs/core/prefab.hpp"

namespace demo::ecs {
    struct map_controller : entity_builder {
    public:
        map_controller(prefab_id wall_prefab) :
            m_wall_prefab(wall_prefab)
        {
        }

        entt::entity build(entt::registry& registry, const asset_context& ctx) override {
            auto entity = registry.create();
            
            registry.emplace<map_control>(entity);

            // positional
            registry.emplace<static_entity>(entity);
            registry.emplace<world_position>(entity);

            // visual
            registry.emplace<tile_renderer>(entity, tile_renderer {
                .tex = &ctx.test.floor
            });

            registry.emplace<defence_placer>(entity, defence_placer {
                .wall_prefab = m_wall_prefab
            });

            registry.emplace<base_defence_zone>(entity);

            return entity;
        }

    private:
        prefab_id m_wall_prefab;
    };
}
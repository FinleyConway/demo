#pragma once

#include "ecs/core/entity_builder.hpp"
#include "ecs/core/prefab.hpp"

#include "ecs/features/map/components/map.hpp"
#include "ecs/features/positioning/components/position.hpp"
#include "ecs/features/rendering/components/tile_renderer.hpp"
#include "ecs/features/positioning/components/static_entity.hpp"
#include "ecs/features/settlement_builder/components/structures.hpp"
#include "ecs/features/settlement_builder/components/settlement_zone.hpp"

namespace demo::ecs {
    struct map_controller : entity_builder {
    public:
        map_controller(prefab_id wall_prefab) :
            m_wall_prefab(wall_prefab)
        {
        }

        entt::entity build(entt::registry& registry, const asset_context& ctx) override {
            auto entity = registry.create();

            // positional
            registry.emplace<static_entity>(entity);
            registry.emplace<position>(entity);

            registry.emplace<map_init>(entity);
            registry.emplace<map>(entity);

            // visual
            registry.emplace<tile_renderer>(entity, tile_renderer {
                .tex = &ctx.test.floor
            });

            // settlement building
            registry.emplace<structures>(entity, structures {
                .wall_prefab = m_wall_prefab
            });
            registry.emplace<settlement_zone>(entity);

            return entity;
        }

    private:
        prefab_id m_wall_prefab;
    };
}
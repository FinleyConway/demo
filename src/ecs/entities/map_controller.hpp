#pragma once

#include "ecs/core/entity_builder.hpp"
#include "ecs/core/prefab.hpp"

#include "ecs/features/map/map_setup.hpp"
#include "ecs/features/positioning/positioning_setup.hpp"
#include "ecs/features/rendering/components/tile_renderer.hpp"
#include "ecs/features/settlement_builder/settlement_builder_setup.hpp"

namespace demo::ecs {
    struct map_controller : entity_builder {
    public:
        map_controller(prefab_id wall_prefab, prefab_id turret_prefab) :
            m_wall_prefab(wall_prefab),
            m_turret_prefab(turret_prefab)
        {
        }

        entt::entity build(entt::registry& registry, const asset_context& ctx) override {
            auto entity = registry.create();

            // positional
            positioning_feature::make_positioning(registry, entity, {
                .is_static = true,
                .position = { 0, 0 }
            });

            map_feature::make_map(registry, entity);

            // visual
            registry.emplace<tile_renderer>(entity, tile_renderer {
                .tex = &ctx.test.floor
            });

            // settlement building
            settlement_feature::make_map(registry, entity, {
                .wall_prefab = m_wall_prefab,
                .turret_prefab = m_turret_prefab,
            });

            return entity;
        }

    private:
        prefab_id m_wall_prefab;
        prefab_id m_turret_prefab;
    };
}
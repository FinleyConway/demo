#pragma once

#include "ecs/core/entity_builder.hpp"

#include "ecs/features/positioning/positioning_setup.hpp"
#include "ecs/features/ui/ui.hpp"

namespace demo::ecs {
    struct canvas : entity_builder {
    public:
        entt::entity build(entt::registry& registry, const asset_context& ctx) override {
            auto entity = registry.create();

            // positional
            positioning_feature::make_positioning(registry, entity, {
                .is_static = true,
                .position = { 0, 0 }
            });

            // ui
            ui_feature::make_canvas(registry, entity, {
                .canvas_size = { 1920, 1080 }
            });

            return entity;
        }
    };
}
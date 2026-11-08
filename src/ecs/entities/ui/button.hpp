#pragma once

#include "ecs/core/entity_builder.hpp"

#include "ecs/features/positioning/positioning_setup.hpp"
#include "ecs/features/ui/ui.hpp"

namespace demo::ecs {
    struct button : entity_builder {
    public:
        entt::entity build(entt::registry& registry, const asset_context& ctx) override {
            auto entity = registry.create();

            // positional
            positioning_feature::make_positioning(registry, entity, {
                .is_static = true,
                .position = { 100, 100 }
            });

            // ui
            ui_feature::make_button(registry, entity, {
                .sprite_texture = &ctx.test.floor,
                .idle_sprite_source = { 64, 0, 32, 32 },
                .hover_sprite_source = { 64, 0, 32, 32 },
                .pressed_sprite_source = { 64, 0, 32, 32 },
                .button_size = { 32.0f, 32.0f }
            });

            return entity;
        }
    };
}
#pragma once

#include "ecs/core/entity_builder.hpp"

#include "ecs/features/positioning/positioning_setup.hpp"
#include "ecs/features/ui/ui.hpp"

namespace demo::ecs {
    struct button : entity_builder {
    public:
        explicit button(entt::entity canvas) : m_canvas(canvas) {
        }

        entt::entity build(entt::registry& registry, const asset_context& ctx) override {
            auto entity = registry.create();

            // positional
            positioning_feature::make_positioning(registry, entity, {
                .is_static = false,
                .position = { 100, 100 }
            });

            const canvas_id id = registry.get<ui_canvas>(m_canvas).id;

            ui_feature::make_button(registry, entity, {
                .canvas = id, 
                .size = { 32.0f, 32.0f },
                .anchor_padding = { -128.0f, -128.0f },
                .sprite_texture = &ctx.test.floor,
                .idle_sprite_source = { 64, 0, 32, 32 },
                .hover_sprite_source = { 64, 0, 32, 32 },
                .pressed_sprite_source = { 64, 0, 32, 32 },
            });

            return entity;
        }

    private:
        entt::entity m_canvas;
    };
}
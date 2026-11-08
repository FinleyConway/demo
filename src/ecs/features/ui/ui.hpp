#pragma once

#include "ecs/features/ui/components/ui_anchor.hpp"
#include "ecs/features/ui/components/ui_canvas.hpp"
#include "ecs/features/ui/components/ui_size.hpp"
#include "ecs/features/ui/components/ui_sprite.hpp"
#include "ecs/features/ui/components/ui_state.hpp"

#include "ecs/features/ui/systems/ui_click.hpp"
#include "ecs/features/ui/systems/ui_hover.hpp"
#include "ecs/features/ui/systems/ui_visual_state.hpp"
#include "ecs/features/ui/systems/update_canvas_size.hpp"
#include "ecs/features/ui/systems/update_ui_position.hpp"

#include "world/world.hpp"

namespace demo::ecs {
    struct ui_canvas_spec {
        point canvas_size;
    };

    struct ui_button_spec {
        canvas_id canvas = 0;

        point size;

        ui_anchor_point::anchor anchor_point = ui_anchor_point::anchor::bottom_right;
        point anchor_padding;

        const texture* sprite_texture = nullptr;

        rect_int idle_sprite_source;
        rect_int hover_sprite_source;
        rect_int pressed_sprite_source;

        colour sprite_tint = colour::white;
    };

    struct ui_feature {
        static canvas_id make_canvas(entt::registry& registry, entt::entity entity, const ui_canvas_spec& spec) {
            const auto& canvas = registry.emplace<ui_canvas>(entity);
            
            registry.emplace<ui_size>(entity, ui_size {
                .value = spec.canvas_size
            });

            registry.emplace<ui_canvas_capture>(entity);

            return canvas.id;
        }

        static void make_button(entt::registry& registry, entt::entity entity, const ui_button_spec& spec) {
            registry.emplace<ui_element>(entity, ui_element {
                .canvas = spec.canvas
            });

            registry.emplace<ui_size>(entity, ui_size {
                .value = spec.size
            });

            registry.emplace<ui_anchor_point>(entity, ui_anchor_point {
                .value = spec.anchor_point
            });
            registry.emplace<ui_anchor_padding>(entity, ui_anchor_padding {
                .value = spec.anchor_padding
            });

            registry.emplace<ui_sprite>(entity, ui_sprite {
                .tex = spec.sprite_texture,
                .source = spec.idle_sprite_source,
                .tint = spec.sprite_tint,
            });
            registry.emplace<ui_sprite_states>(entity, ui_sprite_states {
                .idle_source = spec.idle_sprite_source,
                .hover_source = spec.hover_sprite_source,
                .pressed_source = spec.pressed_sprite_source
            });

            registry.emplace<ui_interactable>(entity);
            registry.emplace<ui_interaction_state>(entity);
            registry.emplace<ui_state_changed>(entity);
        }

        static void register_systems(world& world) {
            world.add_system<ui_visual_state>();
            
            world.add_system<update_canvas_size>();
            world.add_system<update_ui_position>();

            world.add_system<ui_hover>();
            world.add_system<ui_click>();
        }
    };
}
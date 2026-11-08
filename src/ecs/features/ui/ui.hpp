#pragma once

#include "ecs/features/ui/components/ui_size.hpp"
#include "ecs/features/ui/components/ui_sprite.hpp"
#include "ecs/features/ui/components/ui_state.hpp"

#include "ecs/features/ui/systems/ui_click.hpp"
#include "ecs/features/ui/systems/ui_hover.hpp"
#include "ecs/features/ui/systems/ui_visual_state.hpp"

#include "world/world.hpp"

namespace demo::ecs {
    struct ui_button_spec {
        const texture* sprite_texture = nullptr;

        rect_int idle_sprite_source;
        rect_int hover_sprite_source;
        rect_int pressed_sprite_source;

        colour sprite_tint = colour::white;

        point button_size;
    };

    struct ui_feature {
        static void make_button(entt::registry& registry, entt::entity entity, const ui_button_spec& spec) {
            registry.emplace<ui_size>(entity, ui_size {
                .value = spec.button_size
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
            
            world.add_system<ui_hover>();
            world.add_system<ui_click>();
        }
    };
}
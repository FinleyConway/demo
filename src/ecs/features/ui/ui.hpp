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

#include "ecs/world.hpp"

namespace demo::ecs {
    struct ui_feature {
        using components = entt::type_list<
            ui_anchor_point,
            ui_anchor_padding,
            ui_element,
            ui_canvas,
            ui_canvas_capture,
            ui_size,
            ui_sprite_states,
            ui_interactable,
            ui_interaction_state,
            ui_state_changed
        >;

        //using events = entt::type_list<>;
        
        using systems = entt::type_list<
            ui_hover,
            ui_click,

            ui_visual_state,

            update_canvas_size,
            update_ui_position
        >;

        static void configure(world& world) {
            world.register_component<ui_sprite>(
                [](const nlohmann::json& json, const asset_context& context, prefab_finder& finder){
                    ui_sprite s;
                    
                    if (json.contains("texture")) {
                        s.texture = context.find(json["texture"].get<std::string_view>());
                    }

                    s.source = json["source"];

                    return s;
                }
            );
        }
    };
}
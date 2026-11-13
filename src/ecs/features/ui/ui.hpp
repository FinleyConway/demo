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
    struct ui_feature {
        static void register_to(world& world) {
            world.register_component<ui_anchor_point>();
            world.register_component<ui_anchor_padding>();

            // ui_canvas

            world.register_component<ui_size>();
            world.register_component<ui_sprite_states>();
            world.register_component<ui_interactable>();
            world.register_component<ui_sprite>(
                [](const nlohmann::json& json, const asset_context& context, prefab_finder& finder){
                    ui_sprite s;
                    
                    if (json.contains("texture")) {
                        s.texture = context.find(json["texture"].get<std::string_view>());
                    }

                    s.source = json["source"];

                    return s;
                }
            )
            

            world.register_system<ui_visual_state>();
            
            world.register_system<update_canvas_size>();
            world.register_system<update_ui_position>();

            world.register_system<ui_hover>();
            world.register_system<ui_click>();
        }
    };
}
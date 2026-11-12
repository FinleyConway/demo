#pragma once

#include "ecs/features/rendering/components/animator.hpp"
#include "ecs/features/rendering/components/sprite_renderer.hpp"
#include "ecs/features/rendering/components/tile_renderer.hpp"

#include "ecs/features/rendering/systems/animate_entities.hpp"
#include "ecs/features/rendering/systems/modify_tile_renderer.hpp"
#include "ecs/features/rendering/systems/render_entities.hpp"
#include "ecs/features/rendering/systems/rendering_spatial_index.hpp"
#include "ecs/features/rendering/systems/update_dynamic_renderables.hpp"

#include "world/world.hpp"

namespace demo::ecs {
    struct rendering_feature {
        static void register_to(world& world) {
            world.register_component<animator>(
                [](const nlohmann::json& json, const asset_context& context, prefab_finder& finder){
                    animator a;
                    
                    if (json.contains("clip")) {
                        a.clip = context.find(json["clip"].get<std::string_view>());
                    }

                    a.speed = json["speed"];

                    return a;
                }
            );
            world.register_component<sprite_renderer>(
                [](const nlohmann::json& json, const asset_context& context, prefab_finder& finder){
                    sprite_renderer s;
                    
                    if (json.contains("texture")) {
                        s.texture = context.find(json["texture"].get<std::string_view>());
                    }

                    s.source = json["source"];

                    return s;
                }
            );
            world.register_component<tile_renderer_init>();
            world.register_component<tile_renderer>(
                [](const nlohmann::json& json, const asset_context& context, prefab_finder& finder){
                    tile_renderer t;
                    
                    if (json.contains("texture")) {
                        t.texture = context.find(json["texture"].get<std::string_view>());
                    }
                    return t;
                }
            );
            

            world.register_system<rendering_spatial_index>(world.spatial_grid());
            world.register_system<modify_tile_renderer>();

            world.register_system<animate_entities>();

            world.register_system<update_dynamic_renderables>(world.spatial_grid());
            world.register_system<render_entities>(world.spatial_grid());
        }
    };
}
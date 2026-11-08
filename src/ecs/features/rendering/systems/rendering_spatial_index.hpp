#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/positioning/components/position.hpp"
#include "ecs/features/rendering/components/tile_renderer.hpp"
#include "ecs/features/rendering/components/sprite_renderer.hpp"

#include "core/debugging.hpp"

namespace demo::ecs {
    class rendering_spatial_index : public system_base {
    public:
        rendering_spatial_index(entt::registry& registry, entt::dispatcher& dispatcher, spatial_render_grid& spatial_grid) :
            system_base(registry, dispatcher),
            m_spatial_grid(spatial_grid)
        {
        }

        void on_hook() override {
            // handle added components
            registry().on_construct<sprite_renderer>()
                .connect<&rendering_spatial_index::on_sprite_added>(*this);
            registry().on_construct<tile_renderer>()
                .connect<&rendering_spatial_index::on_tile_added>(*this);

            // handle removing components
            registry().on_destroy<sprite_renderer>()
                .connect<&rendering_spatial_index::on_sprite_remove>(*this);
            registry().on_destroy<tile_renderer>()
                .connect<&rendering_spatial_index::on_tile_remove>(*this);

            // handle component changes
            registry().on_construct<tile_renderer_changed>()
                .connect<&rendering_spatial_index::on_tile_changed>(*this);
        }

    private:
        void on_sprite_added(entt::registry& registry, entt::entity entity) {
            ASSERT(
                (registry.all_of<position, sprite_renderer>(entity)),
                "Sprite component was constructed before world_position"
            );

            const auto& sprite_position = registry.get<position>(entity);
            const auto& sprite = registry.get<sprite_renderer>(entity);
            
            m_spatial_grid.insert(
                { entity, typeid(sprite_renderer) }, 
                sprite.make_bounds(sprite_position.value)
            );
        }

        void on_sprite_remove(entt::registry& registry, entt::entity entity) {
            m_spatial_grid.remove({ entity, typeid(sprite_renderer) });
        }

        void on_tile_added(entt::registry& registry, entt::entity entity) {
            ASSERT(
                (registry.all_of<position, tile_renderer>(entity)),
                "Tile rendering component was constructed before world_position"
            );

            const auto& tiles_position = registry.get<position>(entity);
            const auto& tiles = registry.get<tile_renderer>(entity);
            
            m_spatial_grid.insert(
                { entity, typeid(tile_renderer) }, 
                tiles.make_bounds(tiles_position.value, context<world_grid>())
            );
        }

        void on_tile_changed(entt::registry& registry, entt::entity entity) {
            const auto& tiles_position = registry.get<position>(entity);
            const auto& tiles = registry.get<tile_renderer>(entity);
            
            m_spatial_grid.relocate(
                { entity, typeid(tile_renderer) }, 
                tiles.make_bounds(tiles_position.value, context<world_grid>())
            );
        }

        void on_tile_remove(entt::registry& registry, entt::entity entity) {
            m_spatial_grid.remove({ entity, typeid(tile_renderer) });
        }

    private:
        spatial_render_grid& m_spatial_grid;
    };
}
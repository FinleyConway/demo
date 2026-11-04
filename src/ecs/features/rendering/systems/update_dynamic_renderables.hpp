#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/positioning/components/position.hpp"
#include "ecs/features/rendering/components/tile_renderer.hpp"
#include "ecs/features/positioning/components/static_entity.hpp"
#include "ecs/features/rendering/components/sprite_renderer.hpp"
#include "ecs/features/positioning/components/dirty_position.hpp"

namespace demo::ecs {
    class update_dynamic_renderables : public system_base {
    public:
        update_dynamic_renderables(entt::registry& registry, spatial_render_grid& spatial_grid) :
            system_base(registry),
            m_spatial_grid(spatial_grid)
        {
        }

        void update(float dt) override {
            update_sprite_positions();

            update_tile_positions();

            //? maybe a seperate system to handle this in the feature?
            //? maybe inside positioning
            registry().clear<dirty_position>();
        }

    private:
        void update_sprite_positions() {
            auto view = registry().view<
                position,
                dirty_position,
                sprite_renderer
            >(entt::exclude_t<static_entity>());

            for (auto [e, pos, sprite] : view.each()) {
                m_spatial_grid.relocate(
                    { e, typeid(sprite_renderer) }, 
                    sprite.make_bounds(pos.value)
                );
            }
        }

        void update_tile_positions() {
            auto tile_view = registry().view<
                position,
                dirty_position,
                tile_renderer
            >(entt::exclude_t<static_entity>());

            for (auto [e, pos, tile] : tile_view.each()) {
                m_spatial_grid.relocate(
                    { e, typeid(tile_renderer) }, 
                    tile.make_bounds(pos.value, context<world_grid>())
                );
            }
        }

    private:
        spatial_render_grid& m_spatial_grid;
    };
}
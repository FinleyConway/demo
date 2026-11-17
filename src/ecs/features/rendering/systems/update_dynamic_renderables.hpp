#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/positioning/components/position.hpp"
#include "ecs/features/rendering/components/tile_renderer.hpp"
#include "ecs/features/positioning/components/is_static.hpp"
#include "ecs/features/rendering/components/sprite_renderer.hpp"
#include "ecs/features/positioning/components/dirty_position.hpp"

namespace demo::ecs {
    class update_dynamic_renderables : public system_base {
    public:
        using system_base::system_base;

        static constexpr phase phase_v = phase::render;

    public:
        void update(float dt) override {
            update_sprite_positions();

            update_tile_positions();
        }

    private:
        void update_sprite_positions() {
            auto view = registry().view<
                position,
                dirty_position,
                sprite_renderer
            >(entt::exclude_t<is_static>());

            for (auto [e, pos, sprite] : view.each()) {
                context<spatial_render_grid>().relocate(
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
            >(entt::exclude_t<is_static>());

            for (auto [e, pos, tile] : tile_view.each()) {
                context<spatial_render_grid>().relocate(
                    { e, typeid(tile_renderer) }, 
                    tile.make_bounds(pos.value, context<world_grid>())
                );
            }
        }
    };
}
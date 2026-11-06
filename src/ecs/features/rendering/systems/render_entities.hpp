#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/positioning/components/position.hpp"
#include "ecs/features/rendering/components/tile_renderer.hpp"
#include "ecs/features/rendering/components/sprite_renderer.hpp"

#include "world/grid/aabb.hpp"
#include "rendering/camera.hpp"
#include "rendering/renderer.hpp"
#include "rendering/spatial_id.hpp"

namespace demo::ecs {
    class render_entities : public system_base {
    public:
        render_entities(entt::registry& registry, spatial_render_grid& spatial_grid) :
            system_base(registry),
            m_spatial_grid(spatial_grid)
        {
        }

        void update(float dt) override {
            camera& main_camera = context<camera>();

            const float camera_zoom = main_camera.get_zoom();
            const aabb camera_bounds = main_camera.make_bounds();
            const point camera_offset = {
                .x = GetScreenWidth() / (2.0f * camera_zoom),
                .y = GetScreenHeight() / (2.0f * camera_zoom),
            };

            renderer::draw_in_camera(main_camera, camera_offset, [&]() {
                m_spatial_grid.query(camera_bounds, [&](spatial_id id) {
                    if (id.type == typeid(sprite_renderer)) {
                        draw_sprite(id.entity);
                    }
                    else if (id.type == typeid(tile_renderer)) {
                        draw_grid(id.entity);
                    }
                });
            });
        }

    private:
        void draw_grid(entt::entity entity) {
            const world_grid& grid = context<world_grid>();
            const auto& tiles_position = registry().get<position>(entity);
            const auto& tiles = registry().get<tile_renderer>(entity);

            if (!tiles.visible) return;

            for (const auto& tile : tiles.tiles) {
                const point tile_position = tiles_position.value + grid.cell_to_point(tile.cell_position);

                renderer::draw_sprite(
                    tile_position, 
                    tiles.tex, 
                    tile.source,
                    tile.tint
                );
            }
        }

        void draw_sprite(entt::entity entity) const {
            const auto& sprite_position = registry().get<position>(entity);
            const auto& sprite = registry().get<sprite_renderer>(entity);

            if (!sprite.visible) return;

            renderer::draw_sprite(
                sprite_position.value, 
                sprite.tex, 
                sprite.source,
                sprite.tint
            );
        }

    private:
        spatial_render_grid& m_spatial_grid;
    };
}
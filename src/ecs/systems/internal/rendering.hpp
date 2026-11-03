#pragma once

#include <typeindex> 

#include "ecs/core/system_base.hpp"
#include "ecs/components/positional.hpp"
#include "ecs/components/rendering.hpp"

#include "core/debugging.hpp"
#include "rendering/camera.hpp"
#include "rendering/renderer.hpp"
#include "world/grid/spatial_grid.hpp"
#include "world/world_grid/world_grid.hpp"

namespace demo::ecs {
    struct spatial_id {
        entt::entity entity;
        std::type_index type;

        bool operator==(const spatial_id&) const = default;
    };
}

template<>
struct std::hash<demo::ecs::spatial_id> {
    size_t operator()(const demo::ecs::spatial_id& id) const noexcept {
        const size_t h1 = std::hash<entt::entity>{}(id.entity);
        const size_t h2 = id.type.hash_code();

        return h1 ^ (h2 << 1);
    }
};

namespace demo::ecs {
    class rendering : public system_base {
    public:
        using system_base::system_base;

        void on_hook() override {
            registry().on_construct<sprite>().connect<&rendering::on_sprite_added>(*this);
            registry().on_construct<tile_renderer>().connect<&rendering::on_tile_added>(*this);

            registry().on_destroy<sprite>().connect<&rendering::on_sprite_remove>(*this);
            registry().on_destroy<tile_renderer>().connect<&rendering::on_tile_remove>(*this);
        }

        void render() {
            update_dynamic_entities();

            camera& main_camera = context<camera>();

            const float camera_zoom = main_camera.get_zoom();
            const aabb camera_bounds = make_world_bounds(main_camera);
            const point camera_offset = {
                .x = GetScreenWidth() / (2.0f * camera_zoom),
                .y = GetScreenHeight() / (2.0f * camera_zoom),
            };

            renderer::draw_in_camera(main_camera, camera_offset, [&]() {
                m_spatial_grid.query(camera_bounds, [&](spatial_id id) {
                    if (id.type == typeid(sprite)) {
                        draw_sprite(id.entity);
                    }
                    else if (id.type == typeid(tile_renderer)) {
                        draw_grid(id.entity);
                    }
                });
            });
        }

    private:
        void on_sprite_added(entt::registry& registry, entt::entity entity) {
            ASSERT((registry.all_of<world_position, sprite>(entity)),
                "Sprite component was constructed before world_position");

            const auto& position = registry.get<world_position>(entity);
            const auto& render_data = registry.get<sprite>(entity);
            const aabb sprite_bounds = make_bounds(position, render_data); 
            
            m_spatial_grid.insert({ entity, typeid(sprite) }, sprite_bounds);
        }

        void on_tile_added(entt::registry& registry, entt::entity entity) {
            ASSERT((registry.all_of<world_position, tile_renderer>(entity)),
                "Tile rendering component was constructed before world_position");

            const auto& position = registry.get<world_position>(entity);
            const auto& render_data = registry.get<tile_renderer>(entity);
            const aabb tile_bounds = make_bounds(position, render_data); 
            
            m_spatial_grid.insert({ entity, typeid(tile_renderer) }, tile_bounds);
        }


        void on_sprite_remove(entt::registry& registry, entt::entity entity) {
            m_spatial_grid.remove({ entity, typeid(sprite) });
        }

        void on_tile_remove(entt::registry& registry, entt::entity entity) {
            m_spatial_grid.remove({ entity, typeid(tile_renderer) });
        }

        void update_dynamic_entities() {
            auto view = registry().view<
                dirty_position,
                world_position,
                sprite
            >(entt::exclude_t<static_entity>());

            for (auto [e, pos, sprite] : view.each()) {
                const aabb sprite_bounds = make_bounds(pos, sprite);

                m_spatial_grid.relocate({ e, typeid(sprite) }, sprite_bounds);
                registry().remove<dirty_position>(e);
            }
        }

        constexpr aabb make_bounds(const world_position& pos, const sprite& sprite) const {
            const point& lower_bounds = pos.get(); // i have no pivot system
            const point upper_bounds = {
                .x = lower_bounds.x + sprite.source.w,
                .y = lower_bounds.y + sprite.source.h
            };

            return {
                lower_bounds,
                upper_bounds
            };
        }

        constexpr aabb make_bounds(const world_position& pos, const tile_renderer& tile_renderer) const {
            const auto& tiles = tile_renderer.tiles;
            const point world_pos = pos.get();

            if (tiles.empty()) return { world_pos, {} };

            auto& grid = context<world_grid>();

            point min = world_pos + grid.cell_to_point(tiles.front().position);
            point max = min;

            // produce the bounds from all tiles inside component
            for (const auto& tile : tiles) {
                const point world = world_pos + grid.cell_to_point(tile.position);

                min.x = std::min(min.x, world.x);
                min.y = std::min(min.y, world.y);
                max.x = std::max(max.x, world.x);
                max.y = std::max(max.y, world.y);
            }

            return { min, max };
        }

        constexpr aabb make_world_bounds(const camera& main_camera) {
            const point top_left = main_camera.screen_to_world({
                .x = 0.0f,
                .y = 0.0f,
            });

            const point bottom_right = main_camera.screen_to_world({
                .x = static_cast<float>(GetScreenWidth()),
                .y = static_cast<float>(GetScreenHeight()),
            });

            return {
                std::min(top_left.x, bottom_right.x),
                std::min(top_left.y, bottom_right.y),
                std::max(top_left.x, bottom_right.x),
                std::max(top_left.y, bottom_right.y),
            };
        }

        void draw_grid(entt::entity entity) {
            const world_grid& grid = context<world_grid>();
            const auto& position = registry().get<world_position>(entity);
            const auto& render_data = registry().get<tile_renderer>(entity);

            if (!render_data.visible) return;

            for (const auto& tile : render_data.tiles) {
                const point tile_position = position.get() + grid.cell_to_point(tile.position);

                renderer::draw_sprite(
                    tile_position, 
                    render_data.tex, 
                    tile.source,
                    tile.tint
                );
            }
        }

        void draw_sprite(entt::entity entity) const {
            const auto& position = registry().get<world_position>(entity);
            const auto& render_data = registry().get<sprite>(entity);

            if (!render_data.visible) return;

            renderer::draw_sprite(
                position.get(), 
                render_data.tex, 
                render_data.source,
                render_data.tint
            );
        }

    private:
        static constexpr uint16_t c_cell_size = 32;
        spatial_grid<spatial_id, c_cell_size> m_spatial_grid;
    };
}
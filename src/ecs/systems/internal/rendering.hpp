#pragma once

#include "ecs/core/system_base.hpp"
#include "ecs/components/positional.hpp"
#include "ecs/components/rendering.hpp"

#include "core/debugging.hpp"
#include "rendering/camera.hpp"
#include "rendering/renderer.hpp"
#include "world/grid/world_grid.hpp"
#include "world/grid/spatial_grid.hpp"

namespace demo::ecs {
    class rendering : public system_base {
    public:
        using system_base::system_base;

        void on_hook() override {
            registry().on_construct<sprite>().connect<&rendering::on_rendering_added>(*this);
            registry().on_destroy<sprite>().connect<&rendering::on_rendering_remove>(*this);
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
                draw_grid();

                m_spatial_grid.query(camera_bounds, [&](entt::entity entity) {
                    draw_sprite(entity);
                });
            });
        }

    private:
        void on_rendering_added(entt::registry& registry, entt::entity entity) {
            ASSERT((registry.all_of<world_position, sprite>(entity)),
                "Sprite component was constructed before world_position");

            const auto& position = registry.get<world_position>(entity);
            const auto& render_data = registry.get<sprite>(entity);
            
            m_spatial_grid.insert(entity, make_bounds(position, render_data));
        }

        void on_rendering_remove(entt::registry& registry, entt::entity entity) {
            m_spatial_grid.remove(entity);
        }

        void update_dynamic_entities() {
            auto view = registry().view<
                dirty_position,
                world_position,
                sprite
            >(entt::exclude_t<static_entity>());

            for (auto [e, pos, sprite] : view.each()) {
                m_spatial_grid.relocate(e, make_bounds(pos, sprite));
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

        void draw_grid() {
            const world_grid& grid = context<world_grid>();

            const size_t cellSize = grid.cell_size();

            const int offsetX = 0;
            const int offsetY = 0;

            for (size_t row = 0; row < grid.height(); row++) {
                for (size_t col = 0; col < grid.width(); col++) {

                    // temp rendering for grid for now
                    Color tileColor = ((row + col) % 2 == 0)
                        ? (Color){ 80, 160, 70, 255 }
                        : (Color){ 100, 190, 80, 255 };

                    int x = offsetX + static_cast<int>(col * cellSize);
                    int y = offsetY + static_cast<int>(row * cellSize);
                    int size = static_cast<int>(cellSize);

                    DrawRectangle(x, y, size, size, tileColor);
                    DrawRectangleLines(x, y, size, size, (Color){ 40, 100, 40, 255 });
                }
            }
        }

        void draw_sprite(entt::entity entity) const {
            const auto& position = registry().get<world_position>(entity);
            const auto& render_data = registry().get<sprite>(entity);

            renderer::draw_sprite(
                position.get(), 
                render_data.tex, 
                render_data.source
            );
        }

    private:
        static constexpr uint16_t c_cell_size = 32;
        spatial_grid<entt::entity, c_cell_size> m_spatial_grid;
    };
}
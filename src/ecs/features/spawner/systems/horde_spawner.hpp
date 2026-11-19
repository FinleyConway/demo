#pragma once

#include <array>
#include <optional>

#include "core/input/input.hpp"
#include "core/math/point.hpp"
#include "core/random.hpp"
#include "ecs/core/system_base.hpp"
#include "ecs/features/positioning/components/position.hpp"
#include "ecs/features/spawner/components/enemies.hpp"
#include "rendering/camera.hpp"
#include "world/world_grid/world_grid.hpp"

namespace demo::ecs {
    class horde_spawner : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override {
            auto view = registry().view<
                enemies
            >();

            for (auto [e, enemies] : view.each()) {
                if (input::mouse_button_pressed(input::mouse::left)) {
                    const auto& main_camera = context<camera>();
                    auto& grid = context<world_grid>();

                    const cell cell_position = mouse_to_cell(main_camera, grid);

                    generate_spawn_area(enemies, cell_position, 4.5f, 0.75f, grid);
                }
            }
        }

    private:
        void generate_spawn_area(const enemies& enemies, const cell& cell_offset, float radius, float spawn_chance, world_grid& grid) {
            if (!grid.in_bounds(cell_offset)) return;

            const std::vector<cell> cells = grid.cells_in_radius(radius);

            for (const auto& c : cells) {
                const cell offset = {
                    .x = c.x + cell_offset.x,
                    .y = c.y + cell_offset.y
                };

                if (!grid.in_bounds(offset)) continue;

                // dont place enemies on structures
                if (grid.at(offset).has_structure(registry())) continue;

                const auto& entities_in_cell = grid.at(offset).entities_in_cell();

                // get each quad
                for (size_t i = 0; i < entities_in_cell.size(); i++) {
                    // theres already a enemy inside this quad
                    if (registry().valid(entities_in_cell[i])) continue;

                    // make spawning feel more natural 
                    if (random::chance(spawn_chance)) {
                        // spawn entity from prefab
                        auto e = prefab_instantiate(enemies.horder);

                        // put entity in grid
                        grid.at(offset).add_entity(i, e);

                        // set entity position
                        registry().patch<position>(e, [&](auto& pos) {
                            pos.value = get_quadrant_position(i, offset, grid);
                        });
                    }
                }
            }
        }

        point get_quadrant_position(size_t quad_pos, const cell& cell, const world_grid& grid) {
            const uint32_t quadrant = grid.cell_size() / 2;
            const point world_position = grid.cell_to_point(cell);

            static constexpr std::array<point, 4> offsets{{
                {0, 0},
                {1, 0},
                {0, 1},
                {1, 1}
            }};

            return {
                .x = world_position.x + quadrant * offsets[quad_pos].x,
                .y = world_position.y + quadrant * offsets[quad_pos].y
            };
        }

        std::optional<point> get_available_cell_quadrant(const cell& cell_pos, const world_grid& grid) {
            if (grid.in_bounds(cell_pos)) return std::nullopt;

            const uint32_t quadrant = grid.cell_size() / 2;
            const point world_position = grid.cell_to_point(cell_pos);
            const auto& cells = grid.get(cell_pos).entities_in_cell();

            static constexpr std::array<point, 4> offsets{{
                {0, 0},
                {1, 0},
                {0, 1},
                {1, 1}
            }};

            for (size_t i = 0; i < cells.size(); i++) {
                if (cells[i] == entt::null) {
                    return std::make_optional<point>({
                        .x = world_position.x + quadrant * offsets[i].x,
                        .y = world_position.y + quadrant * offsets[i].y
                    });
                }
            }

            return std::nullopt;
        }

        cell mouse_to_cell(const camera& main_camera, const world_grid& grid) const {
            const point world_position = main_camera.screen_to_world(
                input::mouse_position()
            );

            return grid.point_to_cell(world_position);
        }
    };
}
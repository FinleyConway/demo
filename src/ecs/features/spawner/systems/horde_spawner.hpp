#pragma once

#include <array>
#include <optional>

#include <entt/entt.hpp>

#include "core/math/point.hpp"
#include "ecs/core/system_base.hpp"
#include "world/world_grid/world_grid.hpp"

namespace demo::ecs {
    class horde_spawner : public system_base {
    public:
        using system_base::system_base;

    private:
        std::optional<point> get_available_cell_quadrant(cell cell_pos, uint32_t cellsize, const std::array<entt::entity, 4>& cell) {
            const auto& grid = context<world_grid>();

            const uint32_t quadrant = cellsize / 2;
            const point world_position = grid.cell_to_point(cell_pos);

            static constexpr std::array<point, 4> offsets{{
                {0, 0},
                {1, 0},
                {0, 1},
                {1, 1}
            }};

            for (size_t i = 0; i < cell.size(); i++) {
                if (cell[i] == entt::null) {
                    return std::make_optional<point>({
                        .x = world_position.x + quadrant * offsets[i].x,
                        .y = world_position.y + quadrant * offsets[i].y
                    });
                }
            }

            return std::nullopt;
        }
    };
}
#pragma once

#include <array>
#include <vector>
#include <cstdint>
#include <cstddef>
#include <optional>
#include <algorithm>
#include <unordered_set>

#include "grid/cell.hpp"
#include "math/point.hpp"
#include "utils/debugging.hpp"

namespace demo {
    template<typename T = int, size_t TWidth = 0, size_t THeight = 0, uint32_t TCellSize = 0>
    class grid {
    public:
        constexpr size_t width() const {
            return TWidth;
        }

        constexpr size_t height() const {
            return THeight;
        }

        constexpr uint32_t cell_size() const {
            return TCellSize;
        }

        constexpr T& at(const cell& c) {
            m_changed_cells.emplace(c);

            return m_data[to_index(c)];
        }

        constexpr const T& get(const cell& c) const {
            return m_data[to_index(c)];
        }

        constexpr void set(const cell& c, const T& data) {
            m_changed_cells.emplace(c);            
            m_data[to_index(c)] = data;
        }

        constexpr std::array<std::optional<cell>, 4> vonn_neighbours(const cell& c) const {
            return {{
                c.y > 0
                    ? std::optional<cell>{{c.x, c.y - 1}}
                    : std::nullopt,  // up

                c.x + 1 < TWidth
                    ? std::optional<cell>{{c.x + 1, c.y}}
                    : std::nullopt,  // right

                c.y + 1 < THeight
                    ? std::optional<cell>{{c.x, c.y + 1}}
                    : std::nullopt,  // down

                c.x > 0
                    ? std::optional<cell>{{c.x - 1, c.y}}
                    : std::nullopt   // left
            }};
        }

        constexpr bool has_changed() const {
            return !m_changed_cells.empty();
        }

        constexpr std::unordered_set<cell> consume_changed_cells() {
            std::unordered_set<cell> changed;
            changed.swap(m_changed_cells);
            
            return changed;
        }

        static constexpr std::vector<spatial_cell> cells_in_radius(float radius) {
            std::vector<spatial_cell> cells;

            if (radius < 0.0f) return cells;

            const int64_t extent = static_cast<int64_t>(std::floor(radius));
            const float radius_squared = radius * radius;

            for (int64_t y = -extent; y <= extent; ++y) {
                for (int64_t x = -extent; x <= extent; ++x) {
                    if (static_cast<float>(x * x + y * y) <= radius_squared) {
                        cells.emplace_back(x, y);
                    }
                }
            }

            return cells;
        }

        static constexpr std::vector<spatial_cell> cells_in_sector(uint16_t angle, uint16_t width, float radius) {
            std::vector<spatial_cell> cells = cells_in_radius(radius);

            std::erase_if(cells, [&](const spatial_cell& c) {
                const float dx = static_cast<float>(c.x);
                const float dy = static_cast<float>(c.y);

                const float cell_angle = std::atan2(dy, dx) * 180.0f / std::numbers::pi_v<float>;
                const float difference = std::fmod(cell_angle - angle + 540.0f, 360.0f) - 180.0f;

                return std::abs(difference) > width / 2.0f;
            });

            return cells;
        }

        static constexpr bool in_bounds(const cell& c) {
            return c.x < TWidth && c.y < THeight;
        }

        static constexpr size_t manhattan_distance(const cell& a, const cell& b) {
            const size_t dx = a.x > b.x ? a.x - b.x : b.x - a.x;
            const size_t dy = a.y > b.y ? a.y - b.y : b.y - a.y;

            return dx + dy;
        }

        static constexpr point cell_to_point(const cell& cell) {
            return {
                (cell.x * static_cast<float>(TCellSize)),
                (cell.y * static_cast<float>(TCellSize))
            };
        }

        static constexpr cell point_to_cell(const point& world_pos) {
            return {
                static_cast<size_t>(std::floor(world_pos.x / TCellSize)),
                static_cast<size_t>(std::floor(world_pos.y / TCellSize))
            };
        }

    private:
        static constexpr size_t to_index(const cell& c) {
            ASSERT(in_bounds(c), "Out of bounds!");

            return c.y * TWidth + c.x;
        }

    private:
        std::array<T, TWidth * THeight> m_data{};  
        std::unordered_set<cell> m_changed_cells;
    };
}
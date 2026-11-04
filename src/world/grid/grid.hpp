#pragma once

#include <array>
#include <cmath>
#include <vector>
#include <cstdint>
#include <numbers> 
#include <cstddef>
#include <optional>

#include "core/debugging.hpp"
#include "core/math/point.hpp"
#include "world/grid/cell.hpp"

namespace demo {
    template<typename T = int, size_t TWidth = 0, size_t THeight = 0, uint32_t TCellSize = 0>
    class grid {
        static_assert(TCellSize > 0, "TCellSize must be greater than zero");

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
            return m_data[to_index(c)];
        }

        constexpr const T& get(const cell& c) const {
            return m_data[to_index(c)];
        }

        constexpr void set(const cell& c, const T& data) {
            m_data[to_index(c)] = data;
        }

        constexpr std::array<std::optional<cell>, 4> vonn_neighbours(const cell& c) const {
            const cell up    { c.x,     c.y - 1 };
            const cell right { c.x + 1, c.y     };
            const cell down  { c.x,     c.y + 1 };
            const cell left  { c.x - 1, c.y     };

            return {{
                in_bounds(up)    ? std::optional{up}    : std::nullopt,
                in_bounds(right) ? std::optional{right} : std::nullopt,
                in_bounds(down)  ? std::optional{down}  : std::nullopt,
                in_bounds(left)  ? std::optional{left}  : std::nullopt
            }};
        }

        static constexpr std::vector<cell> cells_in_radius(float radius) {
            std::vector<cell> cells;

            if (radius < 0.0f) return cells;

            const int32_t extent = static_cast<int32_t>(std::floor(radius));
            const float radius_squared = radius * radius;

            for (int32_t y = -extent; y <= extent; y++) {
                for (int32_t x = -extent; x <= extent; x++) {
                    if (static_cast<float>(x * x + y * y) <= radius_squared) {
                        cells.emplace_back(x, y);
                    }
                }
            }

            return cells;
        }

        static constexpr std::vector<cell> cells_in_sector(uint16_t angle, uint16_t width, float radius) {
            std::vector<cell> cells = cells_in_radius(radius);

            std::erase_if(cells, [&](const cell& c) {
                const float dx = static_cast<float>(c.x);
                const float dy = static_cast<float>(c.y);

                const float cell_angle = std::atan2(dy, dx) * 180.0f / std::numbers::pi_v<float>;
                const float difference = std::fmod(cell_angle - angle + 540.0f, 360.0f) - 180.0f;

                return std::abs(difference) > width / 2.0f;
            });

            return cells;
        }

        static constexpr bool in_bounds(const cell& c) {
            return c.x >= 0 &&
                   c.y >= 0 &&
                   static_cast<size_t>(c.x) < TWidth &&
                   static_cast<size_t>(c.y) < THeight;
        }

        static constexpr int64_t manhattan_distance(const cell& a, const cell& b) {
            const int64_t dx = std::abs(static_cast<int64_t>(a.x) - b.x);
            const int64_t dy = std::abs(static_cast<int64_t>(a.y) - b.y);

            return dx + dy;
        }

        static constexpr point cell_to_point(const cell& cell) {
            return {
                (cell.x * static_cast<float>(TCellSize)),
                (cell.y * static_cast<float>(TCellSize))
            };
        }

        static constexpr cell point_to_cell(const point& world_pos) {
            static_assert(TCellSize > 0);

            return {
                static_cast<int32_t>(std::floor(world_pos.x / TCellSize)),
                static_cast<int32_t>(std::floor(world_pos.y / TCellSize))
            };
        }

        template<typename Fn>
        static constexpr void for_each_cell(const cell& offset, const cell& size, Fn&& fn) {
            for (int32_t x = 0; x < size.x; x++) {
                for (int32_t y = 0; y < size.y; y++) {
                    fn(cell(x + offset.x, y + offset.y));
                }
            }
        }

    private:
        static constexpr size_t to_index(const cell& c) {
            ASSERT(in_bounds(c), "Out of bounds!");

            return static_cast<size_t>(c.y) * TWidth
                 + static_cast<size_t>(c.x);
        }

    private:
        std::array<T, TWidth * THeight> m_data{};  
    };
}
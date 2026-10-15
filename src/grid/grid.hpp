#pragma once

#include <array>
#include <cstdint>
#include <cassert>
#include <cstddef>
#include <optional>
#include <unordered_set>

#include "grid/position/cell.hpp"
#include "grid/position/point.hpp"

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
            assert(in_bounds(c) && "Out of bounds!");

            return c.y * TWidth + c.x;
        }

    private:
        std::array<T, TWidth * THeight> m_data{};  
        std::unordered_set<cell> m_changed_cells;
    };
}
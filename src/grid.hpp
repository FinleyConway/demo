#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <utility>
#include <optional>

#include "cell.hpp"

namespace demo {
    template<typename T, size_t TWidth, size_t THeight>
    class grid {
    public:
        constexpr T& at(const cell& c) {
            return m_data[to_index(c)];
        }

        constexpr const T& at(const cell& c) const {
            return m_data[to_index(c)];
        }

        constexpr void set(const cell& c, const T& data) {
            m_data[to_index(c)] = data;
        }

        constexpr void set(const cell& c, T&& data) {
            m_data[to_index(c)] = std::move(data);
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

        static constexpr bool in_bounds(const cell& c) {
            return c.x < TWidth && c.y < THeight;
        }

        static constexpr size_t manhattan_distance(const cell& a, const cell& b) {
            const size_t dx = a.x > b.x ? a.x - b.x : b.x - a.x;
            const size_t dy = a.y > b.y ? a.y - b.y : b.y - a.y;

            return dx + dy;
        }

    private:
        static constexpr size_t to_index(const cell& c) {
            assert(in_bounds(c) && "Out of bounds!");

            return c.y * TWidth + c.x;
        }

    private:
        std::array<T, TWidth * THeight> m_data{};
    };
}
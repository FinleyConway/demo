#pragma once

#include <cmath>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

#include "grid/cell.hpp"
#include "grid/aabb.hpp"

namespace demo {
    template<typename T, uint16_t TCellSize> 
    class spatial_grid {
        static_assert(TCellSize > 0, "TCellSize must be greater than zero");

    public:
        void insert(const T& data, const aabb& bounds) {
            // data is unique inside spatial_grid
            if (m_lookup.contains(data)) return;

            get_region_cells(bounds, [&](spatial_cell cell) {
                m_grid[cell].emplace_back(data);
                m_lookup[data].emplace_back(cell);
            });
        }

        void remove(const T& data) {
            // does data exist in grid?
            const auto lookup_it = m_lookup.find(data);
            if (lookup_it == m_lookup.end()) return;

            // find all cells that contain this data
            for (const auto& cell : lookup_it->second) {
                // does the cell exist?
                const auto grid_it = m_grid.find(cell);
                if (grid_it == m_grid.end()) continue; // probably want to log this?

                // get cell data and find data
                auto& cell_data = grid_it->second;
                const auto data_it = std::find( cell_data.begin(), cell_data.end(), data);

                // remove it if found
                if (data_it != cell_data.end()) {
                    cell_data.erase(data_it);
                }

                // clean up cell if no data persists
                if (cell_data.empty()) {
                    m_grid.erase(grid_it);
                }
            }

            m_lookup.erase(lookup_it);
        }

        void relocate(const T& data, const aabb& bounds) {
            remove(data);
            insert(data, bounds);
        }

        template<typename Fn>
        void query(const aabb& bounds, Fn&& fn) {
            std::unordered_set<T> unique;
            
            get_region_cells(bounds, [&](spatial_cell cell) {
                // does the cell exist?
                const auto grid_it = m_grid.find(cell);
                if (grid_it == m_grid.end()) return;

                // get all data inside this cell
                for (const auto& data : grid_it->second) {
                    // have we already captured this data?
                    if (unique.insert(data).second) {
                        fn(data);
                    }
                }
            });
        }

    private:
        template<typename Fn>
        void get_region_cells(const aabb& bounds, Fn&& fn) const {
            if (!bounds.valid()) return;

            const auto [top_left, bottom_right] = make_region(bounds);

            for (auto x = top_left.x;; ++x) {
                for (auto y = top_left.y;; ++y) {
                    fn(spatial_cell(x, y));

                    if (y == bottom_right.y) break;
                }

                if (x == bottom_right.x) break;
            }
        }

        constexpr std::pair<spatial_cell, spatial_cell> make_region(const aabb& bounds) const {
            return {
                to_cell(bounds.lower_bounds),
                to_cell(bounds.upper_bounds)
            };
        }

        constexpr spatial_cell to_cell(const point& point) const {
            return {
                static_cast<int64_t>(std::floor(point.x / TCellSize)),
                static_cast<int64_t>(std::floor(point.y / TCellSize))
            };
        }

    private:
        std::unordered_map<spatial_cell, std::vector<T>> m_grid;
        std::unordered_map<T, std::vector<spatial_cell>> m_lookup;
    };
}
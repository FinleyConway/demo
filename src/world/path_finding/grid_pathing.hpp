#pragma once

#include <queue>
#include <limits>
#include <vector>
#include <optional>
#include <unordered_map>

#include "world/grid/cell.hpp"
#include "world/world_grid/world_grid.hpp"
#include "world/path_finding/movement_profile.hpp"

// https://www.redblobgames.com/pathfinding/a-star/introduction.html

namespace demo {
    struct grid_search {
        const world_grid& grid;
        const cell& start;
        const cell& end;
        const movement_cost& cost;
    };

    class grid_pathing {
    private:
        struct path_node {
            size_t g_cost = std::numeric_limits<size_t>::max();
            size_t h_cost = 0;
            std::optional<cell> parent;
            bool closed = false;

            constexpr size_t f_cost() const {
                return g_cost + h_cost;
            }
        };

        struct entry {
            size_t f_cost = 0;
            cell next;

            constexpr bool operator>(const entry& other) const {
                return f_cost > other.f_cost;
            }
        };

    public:
        static std::vector<cell> search(const grid_search& search) {
            grid_pathing pathfinder(search.grid, search.cost);

            return pathfinder.start(search.start, search.end);
        }

    private:
        std::vector<cell> start(const cell& start, const cell& end) {
            if (!c_world_grid.in_bounds(start) || !c_world_grid.in_bounds(end)) return {};
            if (start == end) return {};

            init_start_node(start, end);

            while (!m_open.empty()) {
                const cell current = m_open.top().next;
                m_open.pop();

                path_node& node = m_nodes.at(current);

                if (node.closed) continue;

                node.closed = true;

                // build path once reach end
                if (current == end) {    
                    return build_path(start, end);
                }

                evaluate_neighbours(current, end);
            }

            // no path exists
            return {};
        }

        void init_start_node(const cell& start, const cell& end) {
            path_node& first = get_or_create_node(start);

            first.g_cost = 0;
            first.h_cost = c_world_grid.manhattan_distance(start, end);

            m_open.push({ first.f_cost(), start });
        }

        void evaluate_neighbours(const cell& current, const cell& end) {
            const size_t current_g = m_nodes.at(current).g_cost;

            for (const auto& neighbour : c_world_grid.vonn_neighbours(current)) {
                if (!neighbour.has_value()) continue;

                path_node& next_node = get_or_create_node(*neighbour);

                const size_t movement_cost = c_movement_cost.cost_for(
                    c_world_grid.get(*neighbour).get_navigation_type()
                );
                const size_t tentative_g = current_g + movement_cost;

                if (tentative_g <= next_node.g_cost) {
                    next_node.g_cost = tentative_g;
                    next_node.h_cost = c_world_grid.manhattan_distance(*neighbour, end);
                    next_node.parent = current;

                    m_open.push({ next_node.f_cost(), *neighbour });
                }
            }
        }

        std::vector<cell> build_path(const cell& start, const cell& goal) const {
            std::vector<cell> path;
            cell current = goal;

            while (current != start) {
                path.emplace_back(current);

                const path_node& node = m_nodes.at(current);

                if (!node.parent.has_value()) return {};

                current = *node.parent;
            }

            path.emplace_back(start);

            // i should return an object that handles the next path
            // rather then providing a backwards cell?
            return path;
        }

        path_node& get_or_create_node(const cell& position) {
            return m_nodes.try_emplace(position).first->second;
        }

    private:
        explicit grid_pathing(const world_grid& world_grid, const movement_cost& movement_cost) :
            c_world_grid(world_grid),
            c_movement_cost(movement_cost)
        {
        } 

    private:
        const world_grid& c_world_grid;
        const movement_cost& c_movement_cost;
        
        std::unordered_map<cell, path_node> m_nodes;
        std::priority_queue<entry, std::vector<entry>, std::greater<>> m_open;
    };
}
#pragma once

#include <queue>
#include <limits>
#include <vector>
#include <optional>
#include <algorithm>
#include <unordered_map>

#include "grid.hpp"

// https://www.redblobgames.com/pathfinding/a-star/introduction.html

namespace demo {
    template<size_t TWidth, size_t THeight>
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

            bool operator>(const entry& other) const {
                return f_cost > other.f_cost;
            }
        };

    public:
        static std::vector<cell> search(
            const grid<int, TWidth, THeight>& world,
            const cell& start,
            const cell& end)
        {
            grid_pathing pathfinder(world);

            return pathfinder.start(start, end);
        }

    public:
        std::vector<cell> start(const cell& start, const cell& end) {
            if (!m_world.in_bounds(start) || !m_world.in_bounds(end)) return {};
            if (start == end) return {};

            init_start_node(start, end);

            while (!m_open.empty()) {
                const auto current = m_open.top().next;
                m_open.pop();

                auto& node = m_nodes.at(current);

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
            auto& first = m_nodes[start];

            first.g_cost = 0;
            first.h_cost = m_world.manhattan_distance(start, end);

            m_open.push({ first.f_cost(), start });
        }

        void evaluate_neighbours(const cell& current, const cell& end) {
            const size_t current_g = m_nodes.at(current).g_cost;

            for (const auto& neighbour : m_world.vonn_neighbours(current)) {
                if (!neighbour.has_value()) continue;

                // 0 means walkable
                if (m_world.at(*neighbour) != 0) continue;

                auto& next_node = m_nodes[*neighbour];
                const size_t tentative_g = current_g + 1;

                if (tentative_g <= next_node.g_cost) {
                    next_node.g_cost = tentative_g;
                    next_node.h_cost = m_world.manhattan_distance(*neighbour, end);
                    next_node.parent = current;

                    m_open.push({ next_node.f_cost(), *neighbour });
                }
            }
        }

        std::vector<cell> build_path(cell start, cell goal) const {
            std::vector<cell> path;
            cell current = goal;

            while (current != start) {
                path.emplace_back(current);

                const auto& node = m_nodes.at(current);

                if (!node.parent.has_value()) return {};

                current = *node.parent;
            }

            path.emplace_back(start);

            // std::reverse(path.begin(), path.end());

            return path;
        }

    private:
        explicit grid_pathing(const grid<int, TWidth, THeight>& world) :
            m_world(world) 
        {
        } 

    private:
        const grid<int, TWidth, THeight>& m_world;
        std::unordered_map<cell, path_node> m_nodes;
        std::priority_queue<entry, std::vector<entry>, std::greater<>> m_open;
    };
}
#pragma once

#include <mutex>
#include <vector>
#include <cstdint>
#include <utility>
#include <unordered_map>

#include "core/debugging.hpp"
#include "core/threading/worker_pool.hpp"

#include "world/grid/cell.hpp"
#include "world/world_grid/world_grid.hpp"
#include "world/path_finding/grid_pathing.hpp"

namespace demo {
    using path_request_id = uint64_t;

    class path_request {
    public:
        path_request() = default;
    
    public:
        bool path_found() const {
            return m_ready;
        }
        
    private:
        explicit path_request(path_request_id id) :
            m_id(id)
        {
        }

    private:
        friend class path_finder;

        path_request_id m_id = 0;
        bool m_ready = false;
    };

    class path_finder {
    public:
        void add_grid_context(const world_grid& world_grid) {
            c_world_grid = &world_grid;
        }   

        path_request request(const cell& start, const cell& end) {
            ASSERT(c_world_grid != nullptr, "No grid context was given");

            // try to start a path
            //? maybe add a lock here if multiple threads access request
            const path_request_id id = m_next_id++; 

            m_pool.add_task(
                [this, start, end]() {
                    return grid_pathing::search(
                        *c_world_grid, start, end
                    );
                },
                [this, id](std::vector<cell>&& path) {
                    on_search_finish(id, std::move(path));
                }
            );

            // return reference id to poll completed tasks
            return path_request(id);
        }        

        std::vector<cell> try_get_result(path_request& request) {
            std::lock_guard lock(m_mutex);

            if (request.m_ready) return {};

            // find and return path if path has been completed
            auto it = m_completed.find(request.m_id);
            if (it == m_completed.end()) return {};

            std::vector<cell> path = std::move(it->second);

            m_completed.erase(it);

            request.m_ready = true;

            return path;
        }

    private:
        void on_search_finish(path_request_id id, std::vector<cell>&& path) {
            std::lock_guard lock(m_mutex);
                    
            m_completed.emplace(
                id,
                std::move(path)
            );
        }

    private:
        const world_grid* c_world_grid;

        std::mutex m_mutex;
        path_request_id m_next_id = 0;
        std::unordered_map<path_request_id, std::vector<cell>> m_completed;
        worker_pool m_pool;
    };
}
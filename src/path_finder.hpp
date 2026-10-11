#pragma once

#include <mutex>
#include <queue>
#include <vector>
#include <cstdint>
#include <utility>
#include <unordered_map>

#include "cell.hpp"
#include "worker_pool.hpp"
#include "grid_pathing.hpp"

namespace demo {
    using path_request_id = uint64_t;

    class path_finder {
    private:
        struct path_request {
            path_request_id request_id = 0;
            cell start;
            cell end;
        };

    public:
        explicit path_finder(const demo::grid<int, 10UL, 10UL>& world) :
            m_world(world) 
        {
        }

    public:
        // dont really like the idea of the main thread handling of creating new workers
        void update() { 
            // poll for any requests made this frame
            while (!m_requests.empty()) {
                const auto& req = m_requests.front();

                // try to start a task or break if this frame has no tasks available
                if (!try_start_task(req.request_id, req.start, req.end)) {
                    break; 
                }

                m_requests.pop();
            }
        }

        path_request_id request(const cell& start, const cell& end) {
            // try to start a path
            const path_request_id id = m_next_id++;
            const bool task_created = try_start_task(id, start, end);

            // wait if all workers are unavailable
            if (!task_created) {
                m_requests.emplace(id, start, end);
            }

            // return reference id to poll completed tasks
            return id;
        }        

        std::optional<std::vector<cell>> try_get_result(path_request_id id) {
            std::lock_guard lock(m_mutex);

            // find and return path if path has been completed
            auto it = m_completed.find(id);
            if (it == m_completed.end()) return std::nullopt;

            std::vector<cell> path = std::move(it->second);

            m_completed.erase(it);

            return std::make_optional(path);
        }

    private:
        bool try_start_task(path_request_id id, const cell& start, const cell& end) {
            const bool task_created = m_pool.add_task(
                [this, start, end]() {
                    return grid_pathing<10, 10>::search(m_world, start, end);
                },
                [this, id](std::vector<cell>&& path) {
                    on_search_finish(id, std::move(path));
                }
            );

            return task_created;
        }

        void on_search_finish(path_request_id id, std::vector<cell>&& path) {
            std::lock_guard lock(m_mutex);
                    
            m_completed.emplace(
                id,
                std::move(path)
            );
        }

    private:
        const demo::grid<int, 10UL, 10UL>& m_world;

        path_request_id m_next_id = 0;

        std::mutex m_mutex;
        std::queue<path_request> m_requests;
        std::unordered_map<path_request_id, std::vector<cell>> m_completed;
        
        worker_pool<std::vector<cell>> m_pool;
    };
}
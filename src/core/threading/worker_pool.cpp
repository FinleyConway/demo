#include "pch.hpp"
#include "core/threading/worker_pool.hpp"

namespace demo {
    worker_pool::worker_pool(size_t workers) {
        // cant allow 0 workers
        if (workers == 0) {
            workers = 1;
        }

        m_workers.reserve(workers);

        // start workers looking for work
        for (size_t i = 0; i < workers; i++) {
            m_workers.emplace_back([this]() {
                worker_loop();
            });
        }
    }

    worker_pool::~worker_pool() {
        {
            std::lock_guard lock(m_mutex);
            m_stopping = true;
        }

        m_condition.notify_all();

        for (auto& worker : m_workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    void worker_pool::worker_loop() {
        while (true) {
            std::function<void()> task;

            {
                std::unique_lock lock(m_mutex);

                // wait for a new task
                m_condition.wait(lock, [this]() {
                    return m_stopping || !m_tasks.empty();
                });

                if (m_stopping && m_tasks.empty()) return;

                // pop task off queue and process task
                task = std::move(m_tasks.front());
                m_tasks.pop();
            }

            task();
        }
    }
}
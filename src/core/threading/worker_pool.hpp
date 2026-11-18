#pragma once

#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <cstddef>
#include <utility>
#include <functional>
#include <condition_variable>

namespace demo {
    class worker_pool {
    public:
        explicit worker_pool(size_t workers = std::thread::hardware_concurrency());

        worker_pool(const worker_pool&) = delete;
        worker_pool& operator=(const worker_pool&) = delete;

        ~worker_pool();

    public: 
        template<typename Task, typename OnFinish>
        bool add_task(Task&& task, OnFinish&& on_finish) {
            {
                std::lock_guard lock(m_mutex);

                if (m_stopping) return false;

                // add work to queue
                m_tasks.emplace(
                    [task = std::forward<Task>(task), on_finish = std::forward<OnFinish>(on_finish)]() {
                        auto result = task();
                        on_finish(std::move(result));
                    }
                );
            }

            // notify a worker waiting that a task has been provided 
            m_condition.notify_one();

            return true;
        }

    private:
        void worker_loop();

    private:
        std::mutex m_mutex;
        std::condition_variable m_condition;

        std::queue<std::function<void()>> m_tasks;
        std::vector<std::thread> m_workers;

        bool m_stopping = false;
    };
}
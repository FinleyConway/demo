#pragma once

#include <vector>
#include <unordered_set>

#include <entt/entt.hpp>

namespace demo::ecs {
    class event_cleanup {
    public:
        template<typename T>
        bool is_tracked() const {
            return m_seen.contains(entt::type_hash<T>::value());
        }

        template<typename T>
        void track() {
            // is T been registed as a cleanup component event?
            if (m_seen.insert(entt::type_hash<T>::value()).second) {
                // create a cleanup callback for this component
                m_cleaners.emplace_back([](entt::registry& r) { r.clear<T>(); });
            }
        }

        void run(entt::registry& registry) {
            for (auto clean : m_cleaners) clean(registry);
        }

    private:
        std::unordered_set<entt::id_type> m_seen;
        std::vector<void(*)(entt::registry&)> m_cleaners;
    };
}
#pragma once

#include <concepts>

#include <entt/entt.hpp>

#include "utils/camera.hpp"

#include "ecs/entity/entity.hpp"

#include "ecs/systems/animation.hpp"
#include "ecs/systems/rendering.hpp"
#include "ecs/systems/system_base.hpp"

namespace demo {
    class world {
    public:
        world() {
            add_context<camera>();
        }

    public:
        template<typename T, typename... Args>
        void add_context(Args&&... args) {
            m_registry.ctx().emplace<T>(
                std::forward<Args>(args)...
            );
        }

        template<typename T>
        void add_system() {
            m_systems.emplace_back(
                std::move(std::make_unique<T>(m_registry))
            );
        }

        template<typename T, typename... Args>
        void add_entity(Args&&... args) {
            static_assert(std::derived_from<T, ecs::entity<T>>);

            T::create(m_registry, std::forward<Args>(args)...);
        }

    public:
        void on_hooks() {
            m_animation.on_hook();
            m_rendering.on_hook();

            for (auto& system : m_systems) {
                system->on_hook();
            }
        }

        void start() {
            m_animation.start();
            m_rendering.start();

            for (auto& system : m_systems) {
                system->start();
            }
        }

        void update(float dt) {
            for (auto& system : m_systems) {
                system->update(dt);
            }

            // internals
            m_animation.update(std::chrono::duration<float>(dt));
        }

        void draw() {
            m_rendering.render();
        }

        void end() {
            for (auto& system : m_systems) {
                system->end();
            }

            m_animation.end();
            m_rendering.end();
        }

    private:
        entt::registry m_registry;

        std::vector<std::unique_ptr<ecs::system_base>> m_systems;
        ecs::animation m_animation{ m_registry };
        ecs::rendering m_rendering{ m_registry };
    };
}
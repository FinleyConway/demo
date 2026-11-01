#pragma once

#include <concepts>

#include <entt/entt.hpp>

#include "ecs/core/entity_builder.hpp"
#include "ecs/core/prefab.hpp"
#include "ecs/core/system_base.hpp"
#include "ecs/core/prefab_manager.hpp"

#include "ecs/systems/internal/animation.hpp"
#include "ecs/systems/internal/rendering.hpp"

#include "core/input/input.hpp"
#include "rendering/camera.hpp"
#include "resources/asset_context.hpp"

namespace demo {
    class world {
    public:
        world() {
            add_context<camera>();
            add_context<ecs::prefab_manager>(m_registry, c_asset_context);
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

        template<typename T>
        void add_entity() {
            static_assert(std::derived_from<T, ecs::entity_builder>);

            T entity;
            entity.build(m_registry, m_registry.ctx().get<asset_context>());
        }

        template<typename T>
        ecs::prefab_id emplace_prefab() {
            auto& ctx = m_registry.ctx().get<ecs::prefab_manager>();

            return ctx.create<T>();
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
            m_input_manager.begin_frame();

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

        input m_input_manager;
        const asset_context c_asset_context;
    };
}
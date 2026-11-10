#pragma once

#include <entt/entt.hpp>

#include "ecs/core/prefab.hpp"
#include "ecs/core/system_base.hpp"
#include "ecs/core/prefab_manager.hpp"

#include "core/input/input.hpp"
#include "rendering/camera.hpp"
#include "resources/asset_context.hpp"
#include "world/world_grid/world_grid.hpp"

namespace demo {
    class world {
    public:
        world() {
            add_context<camera>();
            add_context<ecs::prefab_manager>(m_registry);
        }

    public:
        template<typename T, typename... Args>
        void add_context(Args&&... args) {
            m_registry.ctx().emplace<T>(
                std::forward<Args>(args)...
            );
        }

        template<typename T, typename... Args>
        void add_system(Args&&... args) {
            m_systems.emplace_back(
                std::move(std::make_unique<T>(
                    m_registry, m_dispatcher,
                    std::forward<Args>(args)...
                ))
            );
        }

        //! i hate this but it'll do for now
        spatial_render_grid& spatial_grid() {
            return m_spatial_grid;
        }

    public:
        void on_hooks() {
            for (auto& system : m_systems) {
                system->on_hook();
            }
        }

        void update(float dt) {
            // reset input consumption
            m_input_manager.begin_frame();

            // update entities
            for (auto& system : m_systems) {
                system->update(dt);
            }
        }

    private:
        entt::registry m_registry;
        entt::dispatcher m_dispatcher;

        std::vector<std::unique_ptr<ecs::system_base>> m_systems;

        spatial_render_grid m_spatial_grid;
        input m_input_manager;
        const asset_context c_asset_context;
    };
}
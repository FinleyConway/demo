#pragma once

#include <entt/entt.hpp>

#include "ecs/core/prefab.hpp"
#include "ecs/core/system_base.hpp"
#include "ecs/core/prefab_manager.hpp"
#include "ecs/core/serialisation/entity_loader.hpp"
#include "ecs/core/serialisation/component_registry.hpp"

#include "core/input/input.hpp"
#include "rendering/camera.hpp"
#include "resources/asset_context.hpp"
#include "world/world_grid/world_grid.hpp"

namespace demo {
    class world {
    public:
        world() {
            add_context<asset_context>();
            add_context<ecs::prefab_manager>(m_registry);
            add_context<camera>();
        }

    public:
        template<typename T, typename... Args>
        T& add_context(Args&&... args) {
            return m_registry.ctx().emplace<T>(
                std::forward<Args>(args)...
            );
        }

        template<typename T, typename... Args>
        void register_system(Args&&... args) {
            m_systems.emplace_back(
                std::move(std::make_unique<T>(
                    m_registry, m_dispatcher,
                    std::forward<Args>(args)...
                ))
            );
        }

        template<typename T>
        void register_component() {
            m_component_registry.register_component<T>();
        }

        template<typename T, typename Fn>
        void register_component(Fn&& fn) {
            m_component_registry.register_component<T>(fn);
        }

        //! i hate this but it'll do for now
        spatial_render_grid& spatial_grid() {
            return m_spatial_grid;
        }

    public:
        void load(std::string_view asset_dir, std::string_view prefab_dir, std::string_view entity_dir) {
            auto& assets = m_registry.ctx().get<asset_context>();
            
            assets.load_from_dir(asset_dir);

            ecs::entity_loader_spec spec {
                m_registry,
                m_registry.ctx().get<ecs::prefab_manager>(),
                assets,
                m_component_registry
            };
            ecs::entity_loader loader(spec);

            loader.load(prefab_dir, entity_dir);
        }

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
        ecs::component_registry m_component_registry;

    private:
        input m_input_manager;
        spatial_render_grid m_spatial_grid;
    };
}
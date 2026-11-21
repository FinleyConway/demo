#pragma once

#include <algorithm>

#include <entt/entt.hpp>

#include "ecs/core/system_base.hpp"
#include "ecs/core/event_cleanup.hpp"
#include "ecs/core/prefab_manager.hpp"
#include "ecs/core/serialisation/entity_loader.hpp"
#include "ecs/core/serialisation/component_registry.hpp"
#include "ecs/features/entity_init/components/entity_spawned.hpp"

#include "core/input/input.hpp"
#include "rendering/camera.hpp"
#include "resources/asset_context.hpp"
#include "world/world_grid/world_grid.hpp"

namespace demo::ecs {
    struct system_entry {
        phase update_phase;
        std::unique_ptr<system_base> system;

        template<typename T>
        static constexpr phase phase_of() {
            // opt in way of controlling order of systems
            if constexpr (requires { T::phase_v; }) return T::phase_v;
            else return phase::update;   // default
        }
    };

    class world {
    public:
        world() {
            add_context<prefab_manager>(m_registry);
            add_context<event_cleanup>();

            add_context<asset_context>();
            add_context<camera>();
            add_context<spatial_render_grid>();
        }

    public:
        template<typename T, typename... Args>
        T& add_context(Args&&... args) {
            return m_registry.ctx().emplace<T>(
                std::forward<Args>(args)...
            );
        }

        template<typename... Fs>
        void register_features() {
            (register_feature<Fs>(), ...);
        }

        template<typename T, typename... Args>
        void register_system(Args&&... args) {
            m_systems.emplace_back(
                system_entry::phase_of<T>(),
                std::make_unique<T>(m_registry, std::forward<Args>(args)...)
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

    public:
        void load(std::string_view asset_dir, std::string_view prefab_dir, std::string_view entity_dir) {
            auto& assets = m_registry.ctx().get<asset_context>();
            
            assets.load_from_dir(asset_dir);

            entity_loader_spec spec {
                m_registry,
                m_registry.ctx().get<prefab_manager>(),
                assets,
                m_component_registry
            };
            entity_loader loader(spec);

            loader.load(prefab_dir, entity_dir);
        }

        void on_hooks() {
            // sort system based on phase while maintaining the order i created during setup
            std::stable_sort(m_systems.begin(), m_systems.end(),
                [](const auto& a, const auto& b) { 
                    return a.update_phase < b.update_phase; 
                }
            );

            for (auto& entry : m_systems) {
                entry.system->on_hook();
            }
        }

        void update(float dt) {
            // reset input consumption
            m_input_manager.begin_frame();

            // update entities
            bool start_phase_done = false;

            for (auto& entry : m_systems) {
                // clear all newly created entity flag
                if (!start_phase_done && entry.update_phase != phase::start) {
                    m_registry.clear<entity_spawned>();
                    start_phase_done = true;
                }

                entry.system->update(dt);
            }

            // cleanup component events
            m_registry.ctx().get<event_cleanup>().run(m_registry);
        }

    private:
        template<typename F>
        void register_feature() {
            if constexpr (requires { typename F::components; })
                register_component_list(typename F::components{});
            if constexpr (requires { typename F::events; })
                register_event_list(typename F::events{});
            if constexpr (requires { typename F::systems; })
                register_system_list(typename F::systems{});
            if constexpr (requires(world& w) { F::configure(w); })
                F::configure(*this); // for special cases like sprite_renderer serialisation
        }

        template<typename... Ts>
        void register_component_list(entt::type_list<Ts...>) {
            (register_component<Ts>(), ...);
        }

        template<typename... Ts>
        void register_event_list(entt::type_list<Ts...>) {
            (m_registry.ctx().get<event_cleanup>().template track<Ts>(), ...);
        }

        template<typename... Ts>
        void register_system_list(entt::type_list<Ts...>) {
            (register_system<Ts>(), ...);   
        }

    private:
        entt::registry m_registry;
        std::vector<system_entry> m_systems;
        component_registry m_component_registry;
        input m_input_manager;
    };
}
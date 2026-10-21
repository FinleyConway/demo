#pragma once

#include <entt/entt.hpp>

#include "grid/world_grid.hpp"
#include "grid/pathing/path_finder.hpp"

#include "ecs/components/navigation.hpp"
#include "ecs/components/positional.hpp"

namespace demo::ecs {
    class auto_nav_requester {
    public:
        auto_nav_requester(world_grid& grid, entt::registry& registry) :
            m_grid(grid),
            m_registry(registry),
            m_path_finder(grid)
        {
            m_registry.on_update<navigation_target>()
                .connect<&auto_nav_requester::on_target_updated>(*this);
        }
    
    public:
        void update(float dt) {
            auto view = m_registry.view<
                auto_navigation,
                navigation_target,
                navigation_path,
                navigation_request_state
            >();

            if (m_grid.has_changed()) {
                std::unordered_set<cell> changed_cells = m_grid.consume_changed_cells();

                for (auto entity : view) {
                    handle_repathing(entity, changed_cells);
                }
            }

            for (auto [entity, nav_target, nav_path, nav_request] : view.each()) {
                if (!nav_request.request.path_found()) {
                    nav_path.cells = std::move(m_path_finder.try_get_result(nav_request.request));
                }
            } 
        }
        
        static bool set_target(entt::registry& registry, entt::entity entity, const cell& target) {
            if (!has_required_components(registry, entity)) return false;
            
            registry.patch<navigation_target>(
                entity,
                [&target](navigation_target& component) {
                    component.value = target;
                }
            );

            return true;
        }

    private:
        static bool has_required_components(entt::registry& registry, entt::entity entity) {
            return registry.valid(entity)
                && registry.all_of<
                    auto_navigation,
                    world_position,
                    navigation_target,
                    navigation_path,
                    navigation_motion,
                    navigation_request_state
                >(entity);
        }

        void on_target_updated(entt::registry& registry, entt::entity entity) {
            if (!has_required_components(registry, entity)) return;

            auto& pos = registry.get<world_position>(entity);
            auto& target = registry.get<navigation_target>(entity);
            auto& path = registry.get<navigation_path>(entity);
            auto& motion = registry.get<navigation_motion>(entity);
            auto& request = registry.get<navigation_request_state>(entity);

            path.cells.clear();

            motion.elapsed = 0.0f;
            motion.start_position = pos.get();

            request.request = m_path_finder.request(
                m_grid.point_to_cell(pos.get()),
                target.value
            );
        }

        void handle_repathing(entt::entity entity, const std::unordered_set<cell>& changed_cells)  {
            const auto& path = m_registry.get<navigation_path>(entity);

            for (const auto& c : path.cells) { 
                if (!changed_cells.contains(c)) continue; 

                // request a new path
                const auto& target = m_registry.get<navigation_target>(entity);

                set_target(m_registry, entity, target.value);
            }
        }

    private:
        world_grid& m_grid; 
        entt::registry& m_registry;

        path_finder m_path_finder;
    };
}
#pragma once

#include "ecs/systems/system_base.hpp"

#include "ecs/components/navigation.hpp"
#include "ecs/components/positional.hpp"

#include "grid/pathing/path_finder.hpp"
#include "grid/world_grid.hpp"

namespace demo::ecs {
    class auto_nav_requester : public system_base {
    public:
        using system_base::system_base;

        void on_hook() override {
            registry().on_update<navigation_target>()
                .connect<&auto_nav_requester::on_target_updated>(*this);
        }

        void start() override {
            m_path_finder.add_grid_context(context<world_grid>());
        }

        void update(float dt) override {
            auto view = registry().view<
                auto_navigation,
                navigation_target,
                navigation_path,
                navigation_request_state
            >();

            world_grid& grid = context<world_grid>();

            if (grid.has_changed()) {
                std::unordered_set<cell> changed_cells = grid.consume_changed_cells();

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
                context<world_grid>().point_to_cell(pos.get()),
                target.value
            );
        }

        void handle_repathing(entt::entity entity, const std::unordered_set<cell>& changed_cells)  {
            const auto& path = registry().get<navigation_path>(entity);

            for (const auto& c : path.cells) { 
                if (!changed_cells.contains(c)) continue; 

                // request a new path
                const auto& target = registry().get<navigation_target>(entity);

                navigation_target::set_target(registry(), entity, target.value);
            }
        }

    private:
        path_finder m_path_finder;
    };
}
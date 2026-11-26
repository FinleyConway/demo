#pragma once

#include "core/input/input.hpp"
#include "ecs/core/system_base.hpp"
#include "ecs/features/interactable/components/interactable.hpp"
#include "ecs/features/interactable/components/interactor.hpp"
#include "ecs/features/map/components/map.hpp"
#include "entt/entity/entity.hpp"
#include "rendering/camera.hpp"
#include "world/world_grid/world_grid.hpp"

namespace demo::ecs {
    class structure_interact : public system_base {
    public: 
        using system_base::system_base;

        void update(float dt) {
            auto view = registry().view<
                map,
                interactor
            >();
            
            for (auto e : view) {
                if (input::mouse_button_pressed(input::mouse::left)) {
                    LOG_INFO("clicking");

                    select_structure(e);
                }
            }
        }

    private:
        void select_structure(entt::entity interactor) {
            const auto& main_camera = context<camera>();
            auto& grid = context<world_grid>();
            const cell grid_position = mouse_to_cell(main_camera, grid);

            entt::entity target = entt::null;

            if (grid.in_bounds(grid_position)) {
                auto entity = grid.at(grid_position).structure;

                if (registry().valid(entity) && registry().all_of<interactable>(entity)) {
                    target = entity;
                }
            }

            auto& selection = registry().get<interactor_selection>(interactor);

            if (selection.current == target) return;

            const auto previous = selection.current;
            selection.current = target;

            emit<interactor_selection_changed>(interactor, interactor_selection_changed {
                .previous = previous,
                .current  = target,
            });
        }

        cell mouse_to_cell(const camera& main_camera, const world_grid& grid) const {
            const point world_position = main_camera.screen_to_world(
                input::mouse_position()
            );

            return grid.point_to_cell(world_position);
        }
    };
}
#pragma once

#include "ecs/systems/system_base.hpp"

#include "ecs/components/positional.hpp"
#include "ecs/components/health.hpp"

#include "grid/world_grid.hpp"

namespace demo::ecs {
    class entity_killed : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override {
            auto view = registry().view<world_position, killed>();

            for (auto [e, pos] : view.each()) {
                world_grid& grid = context<world_grid>();
                const cell grid_position = grid.point_to_cell(pos.get());

                grid.at(grid_position).remove_entity(e);

                registry().destroy(e);
            }
        }
    };
}
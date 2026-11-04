#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/positioning/components/position.hpp"
#include "ecs/features/health/components/health_events.hpp"

#include "world/world_grid/world_grid.hpp"

namespace demo::ecs {
    class handle_death : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override {
            auto view = registry().view<position, killed>();

            for (auto [e, pos] : view.each()) {
                world_grid& grid = context<world_grid>();
                tile& tile = grid.at(grid.point_to_cell(pos.value));

                if (tile.has_entity(e)) {
                    tile.remove_entity(e);
                }
                else {
                    tile.set_structure(entt::null);
                }

                registry().destroy(e);
            }
        }
    };
}
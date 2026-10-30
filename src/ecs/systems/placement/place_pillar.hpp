#pragma once

#include "ecs/core/system_base.hpp"
#include "ecs/components/positional.hpp"
#include "ecs/components/defence_pillar.hpp"

#include "world/world_grid/world_grid.hpp"

namespace demo::ecs {
    class place_pillar : public system_base {
    public:
        using system_base::system_base;

        void start() override {
            auto view = registry().view<defence_pillar, world_position>();

            for (auto [e, pos] : view.each()) {
                world_grid& grid = context<world_grid>();

                const size_t width = grid.width();
                const size_t height = grid.height();
                const uint32_t cell_size = grid.cell_size();

                const cell centre_position = {
                    .x = (width  / 2),
                    .y = (height / 2),
                };
                const point world_position = {
                    .x = static_cast<float>(centre_position.x * cell_size),
                    .y = static_cast<float>(centre_position.y * cell_size),
                };

                pos.set(world_position, registry(), e);
                grid.at(centre_position).set_structure(e);
            }
        }
    };
}
#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/positioning/components/position.hpp"
#include "ecs/features/settlement_builder/components/settlement_core.hpp"

#include "world/world_grid/world_grid.hpp"

namespace demo::ecs {
    class place_settlement_core : public system_base {
    public:
        using system_base::system_base;

        void start() override {
            auto view = registry().view<
                position, 
                settlement_core_init,
                settlement_core
            >();

            for (auto [e, pos] : view.each()) {
                auto& grid = context<world_grid>();

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

                grid.at(centre_position).set_structure(e);
                registry().emplace_or_replace<set_position>(e, world_position);
            }
        }
    };
}
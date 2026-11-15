#pragma once

#include "ecs/core/prefab_manager.hpp"
#include "ecs/core/system_base.hpp"

#include "ecs/features/map/components/map.hpp"
#include "ecs/features/positioning/components/position.hpp"
#include "ecs/features/settlement_builder/components/structure_size.hpp"
#include "ecs/features/settlement_builder/components/structures.hpp"
#include "world/world_grid/world_grid.hpp"

namespace demo::ecs {
    class setup_map : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override {
            auto view = registry().view<
                map_init, 
                map,
                structures
            >();

            for (auto [e, structures] : view.each()) {
                place_settlement_core(structures);

                registry().remove<map_init>(e);
            }
        }

    private:
        void place_settlement_core(const structures& structures) {
            auto& prefab = context<prefab_manager>();
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
            const auto core = prefab.instantiate(structures.core_prefab);
            const auto& core_size = registry().get<structure_size>(core);

            grid.for_each_cell(centre_position, core_size.value, [core, &grid](cell cell) {
                grid.at(cell).set_structure(core);
            });

            registry().emplace_or_replace<set_position>(core, world_position);
        }
    };
}
#pragma once

#include "ecs/systems/system_base.hpp"

#include "ecs/components/positional.hpp"
#include "ecs/components/defence_turret.hpp"

#include "grid/cell.hpp"
#include "grid/world_grid.hpp"

#include "utils/debugging.hpp"

namespace demo::ecs {
    class generate_attack_area : public system_base {
    public:
        using system_base::system_base;

        void on_hook() override {
            // for when a turret is created
            registry().on_construct<attack_area>()
                .connect<&generate_attack_area::generate>(*this);

            // for when a sector turret turns
            registry().on_construct<attack_area_turn>()
                .connect<&generate_attack_area::turn_turret>(*this);

            // consider if i want to move the turret to a different location?
        }

    private:
        void turn_turret(entt::registry& registry, entt::entity entity) {
            auto* area = registry.try_get<attack_area>(entity);

            ASSERT(area != nullptr, "Trying to turn a non turret type");
            ASSERT(area->shape == attack_shape::sector, "Recieved a turn request by a circle attack turret");

            const world_grid& grid = context<world_grid>();
            const auto& position = registry.get<world_position>(entity);
            const auto& turn = registry.get<attack_area_turn>(entity);

            // check if a change happened
            if (area->angle != turn.angle) {
                // update sector angle and cells
                area->angle = turn.angle;
                area->cells = get_cells(grid, position, *area);
            }

            // remove tag
            registry.remove<attack_area_turn>(entity); // not sure about this due to life time?
        }

        void generate(entt::registry& registry, entt::entity entity) {
            const world_grid& grid = context<world_grid>();
            const auto& position = registry.get<world_position>(entity);
            auto& area = registry.get<attack_area>(entity);

            area.cells = get_cells(grid, position, area);
        }

        std::vector<spatial_cell> get_cells(const world_grid& grid, const world_position& pos, const attack_area& area) {
            switch (area.shape) {
                case attack_shape::circle:
                    return grid.cells_in_radius(area.radius);

                case attack_shape::sector:
                    return grid.cells_in_sector(area.angle, area.width, area.radius);

                default:
                    return {};
            }
        }
    };
}
#pragma once

#include "ecs/core/system_base.hpp"

#include "core/random.hpp"
#include "ecs/features/horde/components/horde_handler.hpp"
#include "ecs/features/horde/components/horde_member.hpp"
#include "ecs/features/navigation/components/requesting/navigation_path.hpp"
#include "ecs/features/positioning/components/position.hpp"
#include "world/world_grid/world_grid.hpp"

namespace demo::ecs {
    class spawn_horde : public system_base {
    public: 
        using system_base::system_base;

        void on_hook() override {
            registry().on_construct<navigation_path>()
                .connect<&spawn_horde::spawn>(*this);
        }

    private:
        void spawn(entt::registry& registry, entt::entity entity) {
            auto [path, info] = registry.get<
                navigation_path, horde_info
            >(entity);

            auto& grid = context<world_grid>();

            generate_spawn_area(entity, info, grid);
        }

        void generate_spawn_area(entt::entity controller, const horde_info& info, world_grid& grid) {
            if (!grid.in_bounds(info.spawn_tile)) return;

            for (const cell& local : grid.cells_in_radius(info.spawn_radius)) {
                // offset the generated cells with the spawn location
                const cell target = {
                    .x = local.x + info.spawn_tile.x,
                    .y = local.y + info.spawn_tile.y
                };

                if (!is_spawnable(target, grid)) continue;

                populate_cell(controller, target, info, grid);
            }
        }

        bool is_spawnable(const cell& target, const world_grid& grid) const {
            // don't place enemies on structures or out of bounds
            return grid.in_bounds(target) && !grid.get(target).has_structure(registry());
        }

        void populate_cell(entt::entity controller, const cell& target, const horde_info& info, world_grid& grid) {
            const auto& slots = grid.at(target).entities_in_cell();

            for (size_t quadrant = 0; quadrant < slots.size(); quadrant++) {
                // there's already an enemy in this quadrant
                if (registry().valid(slots[quadrant])) continue;

                // random chance makes spawning feel more natural
                if (!random::chance(info.spawn_chance)) continue;

                create_horde_member(controller, quadrant, target, info, grid);
            }
        }

        void create_horde_member(entt::entity controller, size_t quadrant, const cell& target, const horde_info& info, world_grid& grid) {
            const auto enemy = prefab_instantiate(info.horde_enemy);

            // give horde member a reference to the controller to get path
            registry().patch<horde_member>(enemy, [&](auto& member) {
                member.horde = controller;
            });

            registry().patch<position>(enemy, [&](auto& pos) {
                pos.value = quadrant_position(quadrant, target, grid);
            });

            grid.at(target).add_entity(quadrant, enemy);
        }

        static point quadrant_position(size_t quadrant, const cell& target, const world_grid& grid) {
            //! give tile from world a static quadrant size
            ASSERT(quadrant < 4, "Received an out of bounds quadrant position!");

            const auto half_cell = grid.cell_size() / 2;
            const point origin = grid.cell_to_point(target);

            return {
                .x = origin.x + half_cell * (static_cast<int32_t>(quadrant % 2)),
                .y = origin.y + half_cell * (static_cast<int32_t>(quadrant / 2))
            };
        }
    };
}
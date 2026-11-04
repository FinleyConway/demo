#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/components/positional.hpp"
#include "ecs/features/combat/components/area_attack.hpp"
#include "ecs/features/combat/components/attack_target.hpp"

#include "core/debugging.hpp"
#include "world/world_grid/world_grid.hpp"

namespace demo::ecs {
    class acquire_attack_target : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override {
            auto view = registry().view<
                world_position,
                attack_area,
                attack_preference
            >();

            for (auto [e, pos, area, pref] : view.each()) {
                // check if still attacking target
                auto* target = registry().try_get<attack_target>(e);

                if (target != nullptr && is_valid_target(e, target->entity, pos, area)) {
                    continue;
                }

                // remove if target is not valid
                if (target != nullptr) {
                    registry().remove<attack_target>(e);
                }

                const auto [found, target_cell] = search_area(e, pos, pref, area);

                if (found == entt::null) continue;

                LOG_INFO("Target found: %d", found);
                
                registry().emplace_or_replace<attack_target>(
                    e,
                    attack_target { 
                        .entity = found,
                        .target_cell = target_cell
                    }
                );
            }
        }

    private:
        bool is_valid_target(entt::entity attacker, entt::entity target, const world_position& pos, const attack_area& area) const {
            if (!registry().valid(target)) return false;
            if (attacker == target) return false;

            // TODO: check that target is still inside attack_area.
            return true;
        }

        std::pair<entt::entity, cell> search_area(entt::entity attacker, const world_position& pos, const attack_preference& pref, const attack_area& area) {
            const world_grid& grid = context<world_grid>();
            const cell origin = grid.point_to_cell(pos.get());

            // simple first come first serve atm
            for (const auto& sc : area.cells) {
                const cell target_cell = sc.to_cell() + origin;

                if (!grid.in_bounds(target_cell)) continue;

                const auto& tile = grid.get(target_cell);

                if (pref.value == attack_preference::type::enemies) {
                    for (const auto entity : tile.entities_in_cell()) {
                        if (entity == attacker) continue;
                        if (!registry().valid(entity)) continue;

                        return { entity, target_cell };
                    }
                }
                else {
                    const auto entity = tile.get_structure();

                    if (entity == entt::null) continue;
                    if (!registry().valid(entity)) continue;

                    return { entity, target_cell };
                }
            }

            return { entt::null, cell{} };
        }   
    };
}
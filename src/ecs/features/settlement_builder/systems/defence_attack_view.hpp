#pragma once

#include "ecs/core/system_base.hpp"
#include "ecs/features/combat/components/attack_area.hpp"
#include "ecs/features/interactable/components/interactor.hpp"
#include "ecs/features/rendering/components/tile_renderer.hpp"
#include "ecs/features/settlement_builder/components/builder_state.hpp"

namespace demo::ecs {
    class defence_attack_view : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override {
            auto view = registry().view<
                interactor_selection_changed, 
                builder_state
            >();

            for (auto [e, change, builder] : view.each()) {
                if (builder.state != builder_state::states::viewing) continue;

                set_visible(change.previous, false);
                set_visible(change.current, true);
            }
        }

        void set_visible(entt::entity structure_entity, bool visible) {
            if (!registry().valid(structure_entity)) return;   
            if (!registry().all_of<attack_area>(structure_entity)) return; 

            if (auto* tiles = registry().try_get<tile_renderer>(structure_entity)) {
                tiles->visible = visible;
            }
        }
    };
}
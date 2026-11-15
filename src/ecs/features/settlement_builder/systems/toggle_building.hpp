#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/map/components/map.hpp"
#include "ecs/features/settlement_builder/components/builder_state.hpp"

namespace demo::ecs {
    class toggle_building : public system_base {
    public:
        using system_base::system_base;

        void on_hook() override {
            dispatcher().sink<on_building_enable>()
                .connect<&toggle_building::on_enable_build_state>();
        }

        void update(float dt) override {

        }

    private:
        void on_enable_build_state(const on_building_enable&) {
            auto view = registry().view<
                map,
                builder_state
            >(); 

            for (auto [e, builder] : view.each()) {
                if (builder.state == builder_state::states::viewing) {
                    builder.state = builder_state::states::building;
                }
                else {
                    builder.state = builder_state::states::viewing;
                }
            }
        }
    };
}
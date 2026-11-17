#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/map/components/map.hpp"
#include "ecs/features/ui/components/ui_canvas.hpp"
#include "ecs/features/ui/components/ui_state.hpp"
#include "ecs/features/settlement_builder/components/builder_state.hpp"

#include "core/debugging.hpp"

namespace demo::ecs {
    class toggle_building_ui : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override {
            auto button_view = registry().view<
                ui_element,
                ui_interactable,
                ui_clicked,
                builder_button
            >();

            for (auto _ : button_view) {
                auto view = registry().view<map, builder_state>();

                for (auto [e, builder] : view.each()) {
                    on_enable_build_state(builder);
                }
            }
        }

    private:
        void on_enable_build_state(builder_state& builder) {
            if (builder.state == builder_state::states::viewing) {
                builder.state = builder_state::states::building;

                LOG_INFO("In building mode");
            }
            else {
                builder.state = builder_state::states::viewing;

                LOG_INFO("In viewing mode");
            }
        }
    };
}
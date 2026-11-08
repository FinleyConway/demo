#pragma once

#include <raylib.h>

#include "ecs/core/system_base.hpp"

#include "ecs/features/ui/components/ui_size.hpp"
#include "ecs/features/ui/components/ui_canvas.hpp"

namespace demo::ecs {
    class update_canvas_size : public system_base {
    public:
        using system_base::system_base;
    
        void update(float dt) {
            auto view = registry().view<ui_canvas, ui_size>();

            for (auto [e, _, size] : view.each()) {
                size.value = {
                    .x = static_cast<float>(GetScreenWidth()),
                    .y = static_cast<float>(GetScreenHeight())
                };
            }
        }
    };
}
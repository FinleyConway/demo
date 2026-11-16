#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/positioning/components/position.hpp"
#include "ecs/features/ui/components/ui_size.hpp"
#include "ecs/features/ui/components/ui_anchor.hpp"
#include "ecs/features/ui/components/ui_canvas.hpp"

namespace demo::ecs {
    class update_ui_position : public system_base {
    public:
        using system_base::system_base;
    
        void update(float dt) {
            //! maybe this should be a event when the window resizes rather then checking each frame
            //! Should be easier in the future as games force games to be in fix ratios that must be changed via ui
            auto view = registry().view<ui_canvas, ui_size>();

            for (auto [e, size] : view.each()) {
                update_elements(e, size);
            }
        }

    private:
        void update_elements(entt::entity canvas, const ui_size& canvas_size) {
            auto view = registry().view<
                ui_anchor_point,
                ui_anchor_padding
            >();

            for (auto [e, anchor_point, anchor_pad] : view.each()) {
                point anchor_positon = get_anchor_positions(anchor_point, canvas_size);
                anchor_positon.x += anchor_pad.padding.x;
                anchor_positon.y += anchor_pad.padding.y;

                registry().patch<position>(e, [&](auto& pos) {
                    pos.value = anchor_positon;
                });
            }
        }

        point get_anchor_positions(const ui_anchor_point& point, const ui_size& canvas_size) {
            switch (point.position) {
                case ui_anchor_point::anchor::top_left:     return { 0, 0 };
                case ui_anchor_point::anchor::top_right:    return { 0, canvas_size.value.x };
                case ui_anchor_point::anchor::bottom_left:  return { canvas_size.value.y, 0 };
                case ui_anchor_point::anchor::bottom_right: return { canvas_size.value.x, canvas_size.value.y };
                case ui_anchor_point::anchor::centre:       return { canvas_size.value.x / 2.0f, canvas_size.value.y / 2.0f };
            }

            return { 0, 0 };
        }
    };
}
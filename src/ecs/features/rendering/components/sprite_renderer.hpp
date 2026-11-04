#pragma once

#include "core/math/rect.hpp"
#include "world/grid/aabb.hpp"
#include "rendering/colour.hpp"
#include "resources/types/texture.hpp"

namespace demo::ecs {
    struct sprite_renderer {
        const texture* tex = nullptr;
        rect_int source;
        colour tint = colour::white;
        bool visible = true;

        constexpr aabb make_bounds(const point& position) const {
            const point& lower_bounds = position; // i have no pivot system
            const point upper_bounds = {
                .x = lower_bounds.x + source.w,
                .y = lower_bounds.y + source.h
            };

            return { lower_bounds, upper_bounds };
        }
    };
}
#pragma once

#include "core/math/rect.hpp"
#include "rendering/colour.hpp"
#include "resources/types/texture.hpp"

namespace demo::ecs {
    struct ui_sprite {
        const texture* tex = nullptr;
        rect_int source;
        colour tint = colour::white;
    };
}
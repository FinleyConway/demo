#pragma once

#include "core/math/rect.hpp"
#include "resources/types/texture.hpp"

namespace demo::ecs {
    struct sprite {
        const texture* tex = nullptr;
        rect_int source;
    };
}
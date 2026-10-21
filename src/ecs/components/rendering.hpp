#pragma once

#include "math/rect.hpp"
#include "resource/types/texture.hpp"

namespace demo::ecs {
    struct sprite {
        const texture* tex = nullptr;
        rect_int source;
    };
}
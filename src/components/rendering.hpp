#pragma once

#include <raylib.h>

#include "math/rect.hpp"
#include "resource/texture.hpp"

namespace demo::ecs {
    struct sprite {
        const texture* tex = nullptr;
        rect_int source;
    };
}
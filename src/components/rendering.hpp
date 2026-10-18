#pragma once

#include <raylib.h>

namespace demo::ecs {
    struct sprite {
        Texture2D texture; // maybe have a handle for ownership?
        Rectangle source;
        Color tint = WHITE;
    };
}
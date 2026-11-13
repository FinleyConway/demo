#pragma once

#include <nlohmann/json.hpp>

#include "core/math/rect.hpp"
#include "rendering/colour.hpp"
#include "resources/asset_context.hpp"

namespace demo::ecs {
    struct ui_sprite {
        asset_id texture = invalid_asset_id;
        rect_int source;
        colour tint = colour::white;
    };
}
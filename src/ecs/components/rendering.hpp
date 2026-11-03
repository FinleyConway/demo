#pragma once

#include <vector>

#include "core/math/rect.hpp"
#include "core/math/point.hpp"
#include "world/grid/cell.hpp"
#include "rendering/colour.hpp"
#include "resources/types/texture.hpp"

namespace demo::ecs {
    struct sprite {
        const texture* tex = nullptr;
        rect_int source;
        colour tint = colour::white;
        bool visible = true;
    };

    struct tile_renderer {
        struct tile {
            cell position;
            rect_int source;
            colour tint = colour::white;
        };

        const texture* tex = nullptr;
        point grid_origin;
        std::vector<tile> tiles;
        bool visible = true;
    };
}
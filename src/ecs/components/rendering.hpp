#pragma once

#include <vector>

#include <entt/entt.hpp>

#include "core/math/rect.hpp"
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

    struct tile_change {};

    struct tile_renderer {
        struct tile {
            cell position;
            rect_int source;
            colour tint = colour::white;
        };

        // could seperate these into a different component and use systems to control the adding
        // use get_or_emplace and then have a system listen and then add a component for renderer changes?
        void add_tile(const tile& tile, entt::registry& registry, entt::entity entity) {
            tiles.emplace_back(tile);
            
            if (!registry.all_of<tile_change>(entity)) {
                registry.emplace<tile_change>(entity);
            }
        }

        const texture* tex = nullptr;
        std::vector<tile> tiles;
        bool visible = true;
    };
}
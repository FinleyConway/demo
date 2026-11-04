#pragma once

#include "world/world.hpp"
#include "ecs/features/rendering/rendering.hpp"

namespace demo::ecs {
    struct rendering_feature {
        static void register_systems(world& world) {
            world.add_system<rendering_spatial_index>(world.spatial_grid());
            world.add_system<modify_tile_renderer>();

            world.add_system<animate_entities>();

            world.add_system<update_dynamic_renderables>(world.spatial_grid());
            world.add_system<render_entities>(world.spatial_grid());
        }
    };
}
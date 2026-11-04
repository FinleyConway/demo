#pragma once

#include "core/math/rect.hpp"
#include "ecs/core/system_base.hpp"

#include "ecs/components/base_management.hpp"
#include "ecs/components/rendering.hpp"

#include "world/world_grid/world_grid.hpp"

namespace demo::ecs {
    class world_grid_setup : public system_base {
    public:
        using system_base::system_base;

        void start() override {
            auto view = registry().view<map_control, tile_renderer>();

            for (auto [e, tile] : view.each()) {
                create_visuals(e, tile);
            }
        }

    private:
        void create_visuals(entt::entity e, tile_renderer& tile) {
            const auto& grid = context<world_grid>();
            const cell grid_size = { grid.width(), grid.height() };

            for_each_cell({}, grid_size, [&](cell cell) {
                rect_int source = ((cell.x + cell.y) % 2 == 0) 
                    ? rect_int(0 , 0, 32, 32)
                    : rect_int(32, 0, 32, 32);

                tile.add_tile(tile_renderer::tile {
                    .cell = cell,
                    .source = source
                }, registry(), e);
            });
        }

        // this could be a helper method inside grid?
        template<typename Fn>
        void for_each_cell(const cell& offset, const cell& size, Fn&& fn) {
            for (size_t x = 0; x < size.x; x++) {
                for (size_t y = 0; y < size.y; y++) {
                    fn(cell(x + offset.x, y + offset.y));
                }
            }
        }
    };
}
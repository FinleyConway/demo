#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/rendering/components/tile_renderer.hpp"

#include "core/debugging.hpp"

namespace demo::ecs {
    class modify_tile_renderer : public system_base {
    public:
        using system_base::system_base;

        void on_hook() override {
            registry().on_construct<add_renderer_tiles>()
                .connect<&modify_tile_renderer::on_add_tiles>(*this);
        }

    private:
        void on_add_tiles(entt::registry& registry, entt::entity entity) {
            ASSERT(
                (registry.all_of<tile_renderer>(entity)),
                "Trying to add tiles to an entity without a tile renderer"
            );

            auto& renderer = registry.get<tile_renderer>(entity);
            auto& request = registry.get<add_renderer_tiles>(entity);

            if (request.values.empty()) {
                registry.remove<add_renderer_tiles>(entity);
                
                return;
            }

            renderer.tiles.insert(
                renderer.tiles.end(),
                request.values.begin(),
                request.values.end()
            );

            registry.emplace_or_replace<tile_renderer_changed>(entity);
            registry.remove<add_renderer_tiles>(entity);
        }
    };
}
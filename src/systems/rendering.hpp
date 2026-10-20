#pragma once

#include <cassert>

#include <entt/entt.hpp>

#include "utils/renderer.hpp"
#include "grid/spatial_grid.hpp"

#include "components/positional.hpp"
#include "components/rendering.hpp"

namespace demo::ecs {
    class rendering {
    public:
        explicit rendering(entt::registry& registry) :
            m_registry(registry)
        {
        }

        void init() {
            // get all rendering entities
            auto view = m_registry.view<
                world_position,
                sprite
            >();

            // place them in the grid
            for (auto [e, pos, sprite] : view.each()) {
                m_spatial_grid.insert(e, make_bounds(pos, sprite));
            }

            // create an event hook for further creation/deletion of entities
            m_registry.on_construct<sprite>().connect<&rendering::on_rendering_added>(*this);
            m_registry.on_destroy<sprite>().connect<&rendering::on_rendering_remove>(*this);
        }

        void render() {
            update_dynamic_entities();

            aabb camera_bounds { 0, 0, 800, 450 }; // need to sort out camera

            m_spatial_grid.query(camera_bounds, [&](entt::entity entity) {
                auto& position = m_registry.get<world_position>(entity);
                auto& render_data = m_registry.get<sprite>(entity);

                renderer::draw_sprite(
                    position.get(), 
                    render_data.tex, 
                    render_data.source
                );
            });
        }   

    private:
        void on_rendering_added(entt::registry& registry, entt::entity entity) {
            assert((registry.all_of<world_position, sprite>(entity)
                && "Sprite component was constructed before world_position"));

            auto& position = registry.get<world_position>(entity);
            auto& render_data = registry.get<sprite>(entity);
            
            m_spatial_grid.insert(entity, make_bounds(position, render_data));
        }

        void on_rendering_remove(entt::registry& registry, entt::entity entity) {
            m_spatial_grid.remove(entity);
        }

        void update_dynamic_entities() {
            auto view = m_registry.view<
                dirty_position,
                world_position,
                sprite
            >(entt::exclude_t<static_entity>());

            for (auto [e, pos, sprite] : view.each()) {
                m_spatial_grid.relocate(e, make_bounds(pos, sprite));
                m_registry.remove<dirty_position>(e);
            }
        }

        constexpr aabb make_bounds(const world_position& pos, const sprite& sprite) const {
            const point& lower_bounds = pos.get(); // i have no pivot system
            const point upper_bounds = {
                .x = lower_bounds.x + sprite.source.w,
                .y = lower_bounds.y + sprite.source.h
            };

            return {
                lower_bounds,
                upper_bounds
            };
        }

    private:
        entt::registry& m_registry;

        static constexpr uint16_t c_cell_size = 32;
        spatial_grid<entt::entity, c_cell_size> m_spatial_grid;
    };
}
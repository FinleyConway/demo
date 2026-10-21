#pragma once

#include <raylib.h>
#include <entt/entt.hpp>

#include "grid/world_grid.hpp"

#include "ecs/components/animation.hpp"
#include "ecs/components/rendering.hpp"
#include "ecs/components/positional.hpp"

#include "ecs/systems/auto_nav_move.hpp"
#include "ecs/systems/auto_nav_requester.hpp"
#include "ecs/systems/animation.hpp"
#include "ecs/systems/rendering.hpp"

#include "resource/test_assets.hpp"

namespace demo {
    class world {
    public:
        world() :
            m_nav_request(m_grid, m_registry),
            m_nav_move(m_grid, m_registry),
            m_rendering(m_registry),
            m_animation(m_registry)
        {
            m_temp_entity = create_navigatable();
        }

    public:
        void update(float dt) {
            Vector2 mouse = GetMousePosition();

            bool mouse_inside =
                mouse.x < m_grid.width() * m_grid.cell_size() &&
                mouse.y < m_grid.height() * m_grid.cell_size();

            size_t mouse_x = 0;
            size_t mouse_y = 0;

            if (mouse_inside) {
                mouse_x = static_cast<size_t>(
                    mouse.x / m_grid.cell_size()
                );
                mouse_y = static_cast<size_t>(
                    mouse.y / m_grid.cell_size()
                );
            }

            if (mouse_inside && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                ecs::auto_nav_requester::set_target(m_registry, m_temp_entity, { mouse_x, mouse_y });
            }

            m_nav_request.update(dt);
            m_nav_move.update(dt);


            m_animation.update(std::chrono::duration<float>(dt));
        }

        void draw() {
            m_rendering.render();
        }
    
    private:
        entt::entity create_navigatable() {
            const auto entity = m_registry.create();

            //m_registry.emplace<ecs::static_entity>(entity);
            m_registry.emplace<ecs::world_position>(entity);
            m_registry.emplace<ecs::sprite>(entity, ecs::sprite {
                &test_asset.source, { 0, 0, 40, 40 }
            });
            m_registry.emplace<ecs::animator>(entity, ecs::animator {
                .clip = &test_asset.walking
            });

            m_registry.emplace<ecs::auto_navigation>(entity);
            m_registry.emplace<ecs::navigation_target>(entity);
            m_registry.emplace<ecs::navigation_path>(entity);
            m_registry.emplace<ecs::navigation_motion>(entity);
            m_registry.emplace<ecs::navigation_request_state>(entity);

            return entity;
        }

    private:
        world_grid m_grid;
        entt::registry m_registry;

    private:
        entt::entity m_temp_entity;
        
        ecs::auto_nav_requester m_nav_request;
        ecs::auto_nav_move m_nav_move;

        ecs::animation m_animation;
        ecs::rendering m_rendering;

        test_assets test_asset;
    };
}
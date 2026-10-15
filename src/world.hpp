#pragma once

#include <iostream>

#include <raylib.h>
#include <entt/entt.hpp>

#include "components/world_position.hpp"
#include "grid/world_grid.hpp"

#include "systems/navigation_system.hpp"

namespace demo {
    class world {
    public:
        world() :
            m_nav_system(m_grid, m_registry)
        {
            m_temp_entity = create_navigatable();
        }

    public:
        void update(float dt) {
            Vector2 mouse = GetMousePosition();

            bool mouse_inside =
                mouse.x < m_grid.width() * m_grid.cell_size() &&
                mouse.y < m_grid.height() * m_grid.cell_size();

            int mouse_x = -1;
            int mouse_y = -1;

            if (mouse_inside) {
                mouse_x = static_cast<int>(
                    mouse.x / m_grid.cell_size()
                );
                mouse_y = static_cast<int>(
                    mouse.y / m_grid.cell_size()
                );
            }

            std::cout << mouse_x << ", " << mouse_y << std::endl;

            if (mouse_inside && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                m_nav_system.set_target(m_temp_entity, { mouse_x, mouse_y });
            }

            m_nav_system.update(dt);
        }

        void draw() {
            auto view = m_registry.view<world_position>();

            for (auto entity : view) {
                const auto& pos = m_registry.get<world_position>(entity);

                DrawRectangle(
                    pos.value.x, pos.value.y, 
                    m_grid.cell_size(), m_grid.cell_size(), 
                    RED
                );
            }
        }
    
    private:
        entt::entity create_navigatable() {
            const auto entity = m_registry.create();

            m_registry.emplace<auto_navigation>(entity);
            m_registry.emplace<world_position>(entity);
            m_registry.emplace<navigation_target>(entity);
            m_registry.emplace<navigation_path>(entity);
            m_registry.emplace<navigation_motion>(entity);
            m_registry.emplace<navigation_request_state>(entity);

            return entity;
        }

    private:
        world_grid m_grid;
        entt::registry m_registry;

    private:
        entt::entity m_temp_entity;
        navigation_system m_nav_system;
    };
}
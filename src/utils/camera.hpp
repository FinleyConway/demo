#pragma once

#include <utility>

#include <raylib.h>

#include "math/point.hpp"

namespace demo {
    class camera {
    public:
        camera() {
            // set some default values
            m_camera.zoom = 1.0f;
        }
    
    public:
        point get_position() const {
            const Vector2& position = m_camera.target;

            return {
                position.x,
                position.y
            };
        }

        void set_position(point world_position) {
            const Vector2 position = {
                .x = world_position.x,
                .y = world_position.y
            };
            
            m_camera.target = position;
        }

        float get_zoom() const {
            return m_camera.zoom;
        }

        void set_zoom(float zoom) {
            if (zoom == 0.0f) return;

            m_camera.zoom = zoom;
        }

        constexpr point screen_to_world(point screen_position) const {
            const Vector2 position = {
                .x = screen_position.x,
                .y = screen_position.y
            };
            const Vector2 world_position = GetScreenToWorld2D(position, m_camera);

            return {
                world_position.x,
                world_position.y
            };
        }

        constexpr point world_to_screen(point world_position) const {
            const Vector2 position = {
                .x = world_position.x,
                .y = world_position.y
            };
            const Vector2 screen_position = GetWorldToScreen2D(position, m_camera);

            return {
                screen_position.x,
                screen_position.y
            };
        }
        
    private:
        void set_offset(const point& offset) {
            m_camera.offset = {
                .x = offset.x,
                .y = offset.y
            };
        }

        template<typename Fn>
        void draw_inside(Fn&& fn) {
            BeginMode2D(m_camera);

            std::forward<Fn>(fn)();

            EndMode2D();
        }

    private:
        friend class renderer;
    
        Camera2D m_camera{};
    };
}
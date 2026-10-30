#pragma once

#include <raylib.h>

#include "core/math/rect.hpp"
#include "core/math/point.hpp"
#include "rendering/camera.hpp"
#include "resources/types/texture.hpp"

namespace demo {
    class renderer {
    public:
        template<typename Fn>
        static void draw_in_camera(camera& camera, const point& offset, Fn&& fn) {
            camera.set_offset(offset);
            camera.draw_inside(fn);
        }

        static void draw_sprite(const point& position, const texture* texture, const rect_int& source, Color tint = WHITE) {
            const Rectangle sprite_source = {
                .x      = static_cast<float>(source.x), 
                .y      = static_cast<float>(source.y),
                .width  = static_cast<float>(source.w), 
                .height = static_cast<float>(source.h)
            };
            const Vector2 sprite_position = {
                .x = position.x,
                .y = position.y
            };

            if (texture != nullptr && texture->valid()) {
                const Texture2D& handle = texture->handle();
                
                DrawTextureRec(handle, sprite_source, sprite_position, tint);
            }
            else {
                DrawRectangle(
                    sprite_position.x, 
                    sprite_position.y, 
                    sprite_source.width, 
                    sprite_source.height, 
                    PURPLE
                );
            }
        } 

    };
}
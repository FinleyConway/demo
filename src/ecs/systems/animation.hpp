#pragma once

#include <entt/entt.hpp>

#include "ecs/components/rendering.hpp"
#include "ecs/components/animation.hpp"

namespace demo::ecs {
    class animation {
    public:
        explicit animation(entt::registry& registry) :
            m_registry(registry)
        {
        }
    
    public:
        void update(std::chrono::duration<float> dt) {
            auto view = m_registry.view<animator, sprite>();

            for (auto [e, anim, sprite] : view.each()) {
                update_animation(anim, sprite, dt);
            }
        }

    private:
        void update_animation(animator& anim, sprite& sprite, std::chrono::duration<float> dt) {
            if (anim.clip == nullptr) return;

            const auto& clip = *anim.clip;

            if (anim.current_frame >= clip.size()) {
                anim.current_frame = 0;
            }

            anim.time += std::chrono::duration_cast<
                std::chrono::milliseconds
            >(dt * anim.speed);

            advance_frame(anim, clip);

            sprite.source = clip.get(anim.current_frame).source;
        }

        void advance_frame(animator& anim, const animation_clip& clip) {
            auto current_frame_duration = clip.get(anim.current_frame).duration;

            while (anim.time >= current_frame_duration) {
                anim.time -= current_frame_duration;
                anim.current_frame++;

                if (anim.current_frame >= clip.size()) {
                    handle_end_of_clip(anim, clip);
                    
                    if (clip.loop_type() != loop_mode::repeat) return;
                }
            }
        }

        void handle_end_of_clip(animator& anim, const animation_clip& clip) {
            if (clip.loop_type() == loop_mode::repeat) {
                anim.current_frame = 0;
            }
            else {
                anim.current_frame = clip.size() - 1;
                anim.time = std::chrono::milliseconds(0);
            }
        }

    private:
        entt::registry& m_registry;
    };
}
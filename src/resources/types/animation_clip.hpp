#pragma once

#include <bits/chrono.h>
#include <chrono>
#include <cassert>
#include <vector>

#include "math/rect.hpp"

namespace demo {
    struct animation_frame {
        rect_int source;
        std::chrono::milliseconds duration;

        animation_frame(const rect_int source, std::chrono::milliseconds duration) :
            source(source),
            duration(duration)
        {
            assert(duration > std::chrono::milliseconds(0) &&
                "Frame is 0 or less duration");
        }
    };
    
    enum class loop_mode {
        once,
        repeat
    };

    class animation_clip {
    public:
        constexpr animation_clip(std::vector<animation_frame>&& frames, loop_mode loop) :
            m_frames(std::move(frames)),
            m_loop(loop)
        {
            assert(!m_frames.empty() && "Must have atleast one frame");

            // calculate duration from given frames
            for (const auto& frame : m_frames) {
                m_duration += frame.duration;
            }
        }

    public:
        constexpr const animation_frame& get(size_t frame) const {
            assert(m_frames.size() > frame && "Out of bounds!");

            return m_frames[frame];
        }

        constexpr loop_mode loop_type() const {
            return m_loop;
        }

        constexpr std::chrono::milliseconds duration() const {
            return m_duration;
        }

        constexpr size_t size() const {
            return m_frames.size();
        }

    private:
        std::vector<animation_frame> m_frames;
        std::chrono::milliseconds m_duration;
        loop_mode m_loop;
    };
}
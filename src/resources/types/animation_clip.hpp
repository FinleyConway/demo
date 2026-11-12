#pragma once

#include <chrono>
#include <cassert>
#include <vector>

#include <nlohmann/json.hpp>

#include "core/math/rect.hpp"
#include "resources/resource_base.hpp"

namespace nlohmann {
    template<>
    struct adl_serializer<std::chrono::milliseconds> {
        static void to_json(json& j, const std::chrono::milliseconds& value) {
            j = value.count();
        }

        static void from_json(const json& j, std::chrono::milliseconds& value) {
            value = std::chrono::milliseconds(j.get<int64_t>());
        }
    };
}

namespace demo {
    struct animation_frame {
        rect_int source;
        std::chrono::milliseconds duration;

        animation_frame(const rect_int& source, std::chrono::milliseconds duration) :
            source(source),
            duration(duration)
        {
            assert(duration > std::chrono::milliseconds(0) &&
                "Frame is 0 or less duration");
        }

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(animation_frame,
            source,
            duration
        );
    };
    
    enum class loop_mode {
        once,
        repeat
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(loop_mode, {
        {loop_mode::once, "once"},
        {loop_mode::repeat, "repeat"},
    });

    class animation_clip : public resource_base {
    public:
        constexpr animation_clip(std::vector<animation_frame>&& frames, loop_mode loop) :
            resource_base(resource_type::animation_clip),
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
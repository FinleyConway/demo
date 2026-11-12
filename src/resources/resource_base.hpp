#pragma once

#include <nlohmann/json.hpp>

namespace demo {
    enum class resource_type {
        none,
        texture,
        animation_clip
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(resource_type, {
        {resource_type::none, "none"},
        {resource_type::texture, "texture"},
        {resource_type::animation_clip, "animation_clip"}
    });

    class resource_base {
    public:
        explicit resource_base(resource_type type) :
            m_resource_type(type)
        {
        } 

        virtual ~resource_base() = default;
    
    public:
        resource_type type() const {
            return m_resource_type;
        }

    private:
        resource_type m_resource_type = resource_type::none;
    };
}
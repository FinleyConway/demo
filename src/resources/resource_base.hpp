#pragma once

namespace demo {
    enum class resource_type {
        none,
        texture,
        animation_clip
    };

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
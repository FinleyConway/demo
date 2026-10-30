#pragma once

namespace demo {
    enum class resource_type {
        none,
        texture
    };
    
    template<typename T>
    class resource {
    public:
        explicit resource(resource_type type) :
            m_resource_type(type)
        {
        } 

        virtual ~resource() = default;
    
    public:
        virtual const T& handle() const = 0;

        resource_type type() const {
            return m_resource_type;
        }

    private:
        resource_type m_resource_type = resource_type::none;
    };
}
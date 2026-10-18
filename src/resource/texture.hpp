#pragma once

#include <string_view>

#include <raylib.h>

#include "resource/resource.hpp"

namespace demo {
    class texture final : public resource<Texture2D> {
    public:
        explicit texture(std::string_view path) :
            resource(resource_type::texture),
            m_resource(LoadTexture(path.data()))
        {
        }
    
        ~texture() {
            if (IsTextureValid(m_resource)) {
                UnloadTexture(m_resource);
            }
        }

    public:
        const Texture2D& handle() const override {
            return m_resource;
        }

    private:
        Texture2D m_resource{};
    };
}
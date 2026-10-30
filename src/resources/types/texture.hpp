#pragma once

#include <cstdint>
#include <string_view>

#include <raylib.h>

#include "resources/types/resource.hpp"

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

        int32_t width() const {
            return m_resource.width;
        }

        int32_t height() const {
            return m_resource.height;
        }

        bool valid() const {
            return IsTextureValid(m_resource);
        }

    private:
        const Texture2D& handle() const override {
            return m_resource;
        }

    private:
        friend class renderer;

        Texture2D m_resource{};
    };
}
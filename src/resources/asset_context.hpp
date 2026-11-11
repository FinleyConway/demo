#pragma once

#include <memory>
#include <vector>
#include <cstddef>

#include "core/debugging.hpp"
#include "resources/resource_base.hpp"
#include "resources/types/texture.hpp"
#include "resources/types/animation_clip.hpp"

namespace demo {
    using asset_id = size_t;

    template<typename T>
    struct resource_traits;

    template<>
    struct resource_traits<texture> {
        static constexpr resource_type type = resource_type::texture;
    };

    template<>
    struct resource_traits<animation_clip> {
        static constexpr resource_type type = resource_type::animation_clip;
    };

    class asset_context {
    public:
        template<typename T, typename... Args>
        asset_id add(Args... args) {
            m_resources.emplace_back(std::unique_ptr<T>(args...));

            return m_resources.size() - 1;
        }

        template<typename T>
        const T& get(asset_id id) const {
            ASSERT(id < m_resources.size(), "Invalid asset_id!");

            const auto& resource = m_resources[id];

            ASSERT(
                resource->type() == resource_traits<T>::type,
                "Asset type mismatch!"
            );

            return static_cast<const T&>(*resource); 
        }

    private:
        std::vector<std::unique_ptr<resource_base>> m_resources;
    };
}
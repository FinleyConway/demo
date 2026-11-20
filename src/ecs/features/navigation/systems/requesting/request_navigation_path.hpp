#pragma once

#include "ecs/core/system_base.hpp"

namespace demo::ecs {
    class request_navigation_path : public system_base {
    public:
        using system_base::system_base;

        void on_hook() override;

    private:
        void on_request_path(entt::registry& registry, entt::entity entity);
    };
}
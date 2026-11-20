#pragma once

#include "ecs/core/system_base.hpp"

namespace demo::ecs {
    class get_navigation_results : public system_base {
    public:
        using system_base::system_base;

        void update(float dt) override;
    };
}
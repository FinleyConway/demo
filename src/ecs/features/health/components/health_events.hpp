#pragma once

namespace demo::ecs {
    struct heal {
        float amount = 0.0f;
    };

    struct take_damage {
        float amount = 0.0f;
    };

    struct killed {};
}
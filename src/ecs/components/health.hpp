#pragma once

namespace demo::ecs {
    struct health {
        float current_health = 100.0f;
        float max_health = 100.0f;
    };

    struct heal {
        float amount = 0.0f;
    };

    struct take_damage {
        float amount = 0.0f;
    };

    struct killed {};
}
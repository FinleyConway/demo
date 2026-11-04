#pragma once

namespace demo::ecs {
    struct health {
        float current = 100.0f;
        float max = 100.0f;
    };

    struct damage {
        float amount = 0.0f;
    };
}
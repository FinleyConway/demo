#pragma once

namespace demo::ecs {
    struct builder_button {};

    struct builder_state {
        enum class states {
            viewing,
            building
        };

        states state = states::viewing;
    };
}
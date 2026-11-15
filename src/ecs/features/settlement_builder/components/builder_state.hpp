#pragma once

namespace demo::ecs {
    struct on_building_enable {};

    struct builder_state {
        enum class states {
            viewing,
            building
        };

        states state;
    };
}
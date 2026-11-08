#pragma once

#include "core/math/point.hpp"

namespace demo::ecs {
    struct ui_anchor_point {
        enum class anchor {
            top_left,
            top_right,
            bottom_left,
            bottom_right,
            centre
        };

        anchor value;
    };

    struct ui_anchor_padding {
        point value;
    };
}
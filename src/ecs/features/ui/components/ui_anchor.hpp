#pragma once

#include <nlohmann/json.hpp>

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

        anchor position = anchor::top_left;

        NLOHMANN_JSON_SERIALIZE_ENUM(anchor, {
            {anchor::top_left, "top_left"},
            {anchor::top_right, "top_right"},
            {anchor::bottom_left, "bottom_left"},
            {anchor::bottom_right, "bottom_right"},
            {anchor::centre, "centre"},
        });
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(ui_anchor_point, position);
    };

    struct ui_anchor_padding {
        point padding;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(ui_anchor_padding, padding);
    };
}
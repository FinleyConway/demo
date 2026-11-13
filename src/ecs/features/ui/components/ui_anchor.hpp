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

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(ui_anchor_point, position);
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(ui_anchor_point::anchor, {
        {ui_anchor_point::anchor::top_left, "top_left"},
        {ui_anchor_point::anchor::top_right, "top_right"},
        {ui_anchor_point::anchor::bottom_left, "bottom_left"},
        {ui_anchor_point::anchor::bottom_right, "bottom_right"},
        {ui_anchor_point::anchor::centre, "centre"},
    });

    struct ui_anchor_padding {
        point padding;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(ui_anchor_padding, padding);
    };
}
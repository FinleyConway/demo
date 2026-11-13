#pragma once

#include <cstdint>

#include <nlohmann/json.hpp>

namespace demo::ecs {
    struct attack_pattern {
        enum class shape {
            none,
            circle,
            sector
        };
        
        shape attack_shape = shape::circle;
        float radius = 0.0f;
        uint16_t angle = 0; // sector direction in degrees
        uint16_t width = 0; // sector width in degrees

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(attack_pattern,
            attack_shape,
            radius,
            angle,
            width
        );
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(attack_pattern::shape, {
        {attack_pattern::shape::none, "none"},
        {attack_pattern::shape::circle, "circle"},
        {attack_pattern::shape::sector, "sector"},
    });
}
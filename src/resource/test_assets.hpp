#pragma once

#include "resource/types/texture.hpp"
#include "resource/types/animation_clip.hpp"

namespace demo {
    using namespace std::chrono_literals;

    struct test_assets {
        texture source;
        animation_clip idle;
        animation_clip walking;

        test_assets() :
            source("/var/home/finley/Projects/Git/demo/test_assets/zombie_stickman.png"),
            idle(
                {
                    { { 0,   0, 32, 32 }, 100ms },
                },
                loop_mode::repeat
            ),
            walking(
                {
                    { { 0,   0, 32, 32 }, 100ms },
                    { { 32,  0, 32, 32 }, 100ms },
                    { { 64,  0, 32, 32 }, 100ms },
                    { { 96,  0, 32, 32 }, 100ms },
                    { { 128, 0, 32, 32 }, 100ms },
                    { { 160, 0, 32, 32 }, 100ms },
                },
                loop_mode::repeat
            )
        {
        }
    };
}
#pragma once

#include "world/path_finding/path_finder.hpp"

namespace demo::ecs {
    struct navigation_request_state {
        path_request request;
    };
    
    struct navigation_wait {};

    struct navigation_failed {};
}
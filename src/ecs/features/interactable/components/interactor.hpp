#pragma once

namespace demo::ecs {
    struct interactor {};

    struct interactor_selection {
        entt::entity current = entt::null;
    };

    struct interactor_selection_changed {
        entt::entity previous = entt::null;
        entt::entity current  = entt::null;
    };
}
#pragma once

#include <entt/entt.hpp>

#include "ecs/features/interactable/components/interactable.hpp"
#include "ecs/features/interactable/components/interactor.hpp"
#include "ecs/features/interactable/systems/structure_interact.hpp"

namespace demo::ecs {
    struct interactable_feature {
        using components = entt::type_list<
            interactor, 
            interactor_selection, 
            interactable
        >;

        using events = entt::type_list<
            interactor_selection_changed
        >;

        using systems = entt::type_list<structure_interact>;
    };
}
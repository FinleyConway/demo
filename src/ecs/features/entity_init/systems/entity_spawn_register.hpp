#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/entity_init/components/entity_spawned.hpp"

namespace demo::ecs {
    class entity_spawn_register : public system_base {
    public:
        using system_base::system_base;

        void on_hook() override {
            emit_register<entity_spawned>();
        }
    }
}
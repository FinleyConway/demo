#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/horde/components/horde_handler.hpp"
#include "ecs/features/navigation/components/requesting/navigation_request.hpp"

namespace demo::ecs {
    class setup_horde : public system_base {
    public: 
        using system_base::system_base;

        void on_hook() override {
            registry().on_construct<horde_target>()
                .connect<&setup_horde::on_create>(*this);
        }

    private:
        void on_create(entt::registry& registry, entt::entity entity) {
            auto [horde, info, count] = registry.get<
                horde_target, horde_info, horde_count
            >(entity);

            // setup horde size
            count.alive = info.enemy_count;   

            // generate path
            emit<navigation_target>(entity, navigation_target {
                .start = info.spawn_tile,
                .end = horde.target
            });
        }
    };
}
#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/health/components/killed.hpp"
#include "ecs/features/horde/components/horde_handler.hpp"
#include "ecs/features/horde/components/horde_member.hpp"

namespace demo::ecs {
    class horde_member_killed : public system_base {
    public: 
        using system_base::system_base;

        void on_hook() override {
            registry().on_construct<killed>()
                .connect<&horde_member_killed::on_killed>(*this);
        }

    private:
        void on_killed(entt::registry& registry, entt::entity entity) {
            if (const auto* member = registry.try_get<horde_member>(entity)) {
                // decrement the horde controller counter
                registry.patch<horde_count>(member->horde, [](auto& horde) {
                    horde.alive -= 1;
                });
            }
        }
    };
}
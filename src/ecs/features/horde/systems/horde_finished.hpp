#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/horde/components/horde_handler.hpp"

namespace demo::ecs {
    class horde_finished : public system_base {
    public: 
        using system_base::system_base;

        void on_hook() override {
            registry().on_update<horde_count>()
                .connect<&horde_finished::on_horde_killed>(*this);
        }

    private:
        void on_horde_killed(entt::registry& registry, entt::entity entity) {
            const auto& counter = registry.get<horde_count>(entity);

            if (counter.alive == 0) {
                // maybe defer entity destruction at the phrase end?
                registry.destroy(entity);
            }
        }
    };
}
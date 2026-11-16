#pragma once

#include "ecs/core/system_base.hpp"

#include "ecs/features/health/components/heal.hpp"
#include "ecs/features/health/components/damage.hpp"
#include "ecs/features/health/components/health.hpp"
#include "ecs/features/health/components/killed_event.hpp"

namespace demo::ecs {
    class manipulate_health : public system_base {
    public:
        using system_base::system_base;

        void on_hook() override {
            dispatcher().sink<damage_event>()
                .connect<&manipulate_health::on_reduce_health>(*this);

            dispatcher().sink<heal_event>()
                .connect<&manipulate_health::on_increase_health>(*this);
        }

    private:
        void on_reduce_health(const damage_event& event) {
            // make sure im working valid entity's
            if (!registry().valid(event.entity)) return;
            if (!registry().all_of<health>(event.entity)) return;

            auto& entity_health = registry().get<health>(event.entity);

            const float previous_health = entity_health.current;

            // apply damage making sure its not negitive
            entity_health.current = std::max(
                0.0f,
                entity_health.current - event.amount
            );

            // if entity has no health, dispatch a killed event
            if (previous_health > 0.0f && entity_health.current <= 0.0f) {
                dispatcher().trigger<killed_event>(
                    killed_event{
                        .entity = event.entity
                    }
                );
            }
        }

        void on_increase_health(const heal_event& event) {
            // make sure im working valid entity's
            if (!registry().valid(event.entity)) return;
            if (!registry().all_of<health>(event.entity)) return;

            auto& entity_health = registry().get<health>(event.entity);

            // should be dead
            if (entity_health.current <= 0.0f) return;

            // apply health clamped within the entities max health
            entity_health.current = std::clamp(
                entity_health.current + event.amount,
                0.0f,
                entity_health.max
            );
        }
    };
}
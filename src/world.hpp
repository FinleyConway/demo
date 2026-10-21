#pragma once

#include <entt/entt.hpp>

#include "utils/camera.hpp"
#include "grid/world_grid.hpp"

#include "ecs/components/animation.hpp"
#include "ecs/components/rendering.hpp"
#include "ecs/components/positional.hpp"

#include "ecs/systems/auto_nav_move.hpp"
#include "ecs/systems/auto_nav_requester.hpp"
#include "ecs/systems/animation.hpp"
#include "ecs/systems/rendering.hpp"

#include "resource/test_assets.hpp"

namespace demo {
    class world {
    public:
        world() :
            m_nav_request(m_grid, m_registry),
            m_nav_move(m_grid, m_registry),
            m_rendering(m_registry, m_main_camera),
            m_animation(m_registry)
        {
            create_navigatable();
        }

    public:
        void update(float dt) {
            m_nav_request.update(dt);
            m_nav_move.update(dt);

            // internals
            m_animation.update(std::chrono::duration<float>(dt));
        }

        void draw() {
            m_rendering.render();
        }
    
    private:
        entt::entity create_navigatable() {
            const auto entity = m_registry.create();

            //m_registry.emplace<ecs::static_entity>(entity);
            m_registry.emplace<ecs::world_position>(entity);
            m_registry.emplace<ecs::sprite>(entity, ecs::sprite {
                &test_asset.source, { 0, 0, 40, 40 }
            });
            m_registry.emplace<ecs::animator>(entity, ecs::animator {
                .clip = &test_asset.walking
            });

            m_registry.emplace<ecs::auto_navigation>(entity);
            m_registry.emplace<ecs::navigation_target>(entity);
            m_registry.emplace<ecs::navigation_path>(entity);
            m_registry.emplace<ecs::navigation_motion>(entity);
            m_registry.emplace<ecs::navigation_request_state>(entity);

            return entity;
        }

    private:
        entt::registry m_registry;

        world_grid m_grid;
        camera m_main_camera;

    private:
        ecs::auto_nav_requester m_nav_request;
        ecs::auto_nav_move m_nav_move;

        ecs::animation m_animation;
        ecs::rendering m_rendering;

        test_assets test_asset;
    };
}
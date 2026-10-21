#pragma once

#include <entt/entt.hpp>

#include "utils/camera.hpp"
#include "grid/world_grid.hpp"

#include "ecs/systems/test_grid_selector.hpp"
#include "ecs/systems/auto_nav_move.hpp"
#include "ecs/systems/auto_nav_requester.hpp"
#include "ecs/systems/animation.hpp"
#include "ecs/systems/rendering.hpp"

#include "ecs/entity/zombie.hpp"

#include "resource/test_assets.hpp"

namespace demo {
    class world {
    public:
        world() :
            m_test(m_registry, m_grid, m_main_camera),
            m_nav_request(m_grid, m_registry),
            m_nav_move(m_grid, m_registry),
            m_rendering(m_registry, m_main_camera),
            m_animation(m_registry)
        {
            ecs::zombie::create(m_registry, m_test_asset);
        }

    public:
        void update(float dt) {
            m_test.update(dt);

            m_nav_request.update(dt);
            m_nav_move.update(dt);

            // internals
            m_animation.update(std::chrono::duration<float>(dt));
        }

        void draw() {
            m_rendering.render();
        }

    private:
        entt::registry m_registry;

        world_grid m_grid;
        camera m_main_camera;

    private:
        ecs::test_grid_selector m_test;

        ecs::auto_nav_requester m_nav_request;
        ecs::auto_nav_move m_nav_move;

        ecs::animation m_animation;
        ecs::rendering m_rendering;

        test_assets m_test_asset;
    };
}
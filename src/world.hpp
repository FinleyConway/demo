#pragma once

#include <entt/entt.hpp>

#include "ecs/components/camera_control.hpp"
#include "ecs/systems/camera_movement.hpp"
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
            m_cam_movement(m_registry, m_main_camera, m_grid),
            m_rendering(m_registry, m_main_camera, m_grid),
            m_animation(m_registry)
        {
            ecs::zombie::create(m_registry, m_test_asset);

            auto e = m_registry.create();
            m_registry.emplace<ecs::camera_control>(e);
        }

    public:
        void update(float dt) {
            m_test.update(dt);

            m_nav_request.update(dt);
            m_nav_move.update(dt);

            m_cam_movement.update(dt);

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
        ecs::camera_movement m_cam_movement;

        ecs::animation m_animation;
        ecs::rendering m_rendering;

        test_assets m_test_asset;
    };
}
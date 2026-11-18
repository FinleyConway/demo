#pragma once

#include <filesystem>
#include <string_view>

#include "resources/asset_context.hpp"
#include "ecs/core/serialisation/prefab_finder.hpp"

namespace demo::ecs {
    class prefab_manager;
    class component_registry;

    struct entity_loader_spec {
        entt::registry& registry;
        prefab_manager& prefabs;

        const asset_context& context;
        const component_registry& components;
    };

    class entity_loader {
    public:
        explicit entity_loader(const entity_loader_spec& spec);

        void load(std::string_view prefab_dir_path, std::string_view entity_dir_path);
        
    private:
        void entity_from_file(const std::filesystem::path& path);

        void prefab_from_file(const std::filesystem::path& path);

    private:
        entity_loader_spec m_spec;
        prefab_finder m_prefab_finder;
    };
}
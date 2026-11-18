#include "pch.hpp"
#include "ecs/core/serialisation/entity_loader.hpp"

#include <fstream>

#include <nlohmann/json.hpp>

#include "core/debugging.hpp"
#include "ecs/core/prefab.hpp"
#include "ecs/core/prefab_manager.hpp"
#include "ecs/core/serialisation/component_registry.hpp"
#include "ecs/features/entity_init/components/entity_spawned.hpp"

namespace demo::ecs {
    entity_loader::entity_loader(const entity_loader_spec& spec)
        : m_spec(spec), m_prefab_finder(spec.prefabs) 
    {
    }

    void entity_loader::load(std::string_view prefab_dir_path, std::string_view entity_dir_path) {
        using namespace std::filesystem;

        // load prefabs first
        for (const auto& dir_entry : recursive_directory_iterator(prefab_dir_path)) {
            if (!dir_entry.is_regular_file()) continue;

            prefab_from_file(dir_entry);
        }

        // then load entities
        for (const auto& dir_entry : recursive_directory_iterator(entity_dir_path)) {
            if (!dir_entry.is_regular_file()) continue;

            entity_from_file(dir_entry);
        }
    }

    void entity_loader::entity_from_file(const std::filesystem::path& path) {
        LOG_INFO("ENTITY: loading %s", path.c_str());

        const auto entity = m_spec.registry.create();

        const auto json = nlohmann::json::parse(std::ifstream(path));
        const auto components = json["components"];

        // add initialisation component
        m_spec.registry.emplace<entity_spawned>(entity);

        // get all components described in json
        for (const auto& [name, data] : components.items()) {
            LOG_INFO("ENTITY: reading %s", name.c_str());
            ASSERT(
                m_spec.components.has_component_info(name),
                "Json component not found!"
            );

            // get component instruction
            const auto& info = m_spec.components.get_component_info(name);
            const auto instruction = info(data, m_spec.context, m_prefab_finder);

            // use instructions to add component
            instruction(m_spec.registry, entity);
        }
    }

    void entity_loader::prefab_from_file(const std::filesystem::path& path) {
        LOG_INFO("PREFAB: loading %s", path.c_str());

        const auto json = nlohmann::json::parse(std::ifstream(path));

        const auto prefab_name = json["name"];
        const auto components = json["components"];

        std::vector<component_registry::instruction> instructions;

        instructions.emplace_back([](entt::registry& r, entt::entity e) {
            r.emplace<entity_spawned>(e);
        });

        // get all components described in json
        for (const auto& [name, data] : components.items()) {
            LOG_INFO("PREFAB: reading %s", name.c_str());
            ASSERT(
                m_spec.components.has_component_info(name),
                "Json component not found!"
            );

            const auto& info = m_spec.components.get_component_info(name);

            // store instructions for execution later
            instructions.emplace_back(info(data, m_spec.context, m_prefab_finder));
        }

        // get or create id from prefab name
        // create prefab with allocated id
        m_spec.prefabs.create(
            m_prefab_finder.get(prefab_name),           
            std::move(instructions)
        );
    }
} 
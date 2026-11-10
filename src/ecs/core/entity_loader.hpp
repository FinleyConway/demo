#pragma once

#include <fstream>
#include <functional>

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "core/debugging.hpp"
#include "ecs/core/prefab_manager.hpp"
#include "resources/asset_context.hpp"

namespace demo::ecs {
    class entity_loader {
    private:
        using instruction = std::function<void(entt::registry&, entt::entity)>;
        using component_info = std::function<instruction(const nlohmann::json&, const asset_context&)>;

    public:
        entity_loader(entt::registry& registry, prefab_manager& prefab_manager, const asset_context& context) :
            m_registry(registry),
            m_prefab_manager(prefab_manager),
            c_context(context)
        {
        }    

        template<typename T>
        void register_component() {
            constexpr std::string_view name = entt::type_name<T>::value();

            m_components.try_emplace(
                name, 
                [](const nlohmann::json& json, const asset_context&) {
                    T component = json.get<T>();

                    return [component = std::move(component)](entt::registry& r, entt::entity e) {
                        r.emplace<T>(e, component);
                    };
                }
            );
        }

        template<typename T, typename Fn>
        void register_component(Fn&& deserialise) {
            constexpr std::string_view name = entt::type_name<T>::value();

            m_components.try_emplace(
                name, 
                [fn = std::forward<Fn>(deserialise)](const nlohmann::json& json, const asset_context& context) {
                    T component = std::forward<Fn>(fn)(json, context);

                    return [component = std::move(component)](entt::registry& r, entt::entity e) {
                        r.emplace<T>(e, component);
                    };
                }
            );
        }

        entt::entity entity_from_file(std::string_view file) const {
            const auto entity = m_registry.create();

            const auto json = nlohmann::json::parse(std::ifstream(file.data()));
            const auto components = json["components"];

            // get all components described in json
            for (const auto& [name, data] : components.items()) {
                ASSERT(m_components.contains(name), "Json component not found!");

                // get component instruction
                const auto& info = m_components.at(name);
                const auto instruction = info(data, c_context);

                // use instructions to add component
                instruction(m_registry, entity);
            }

            return entity;
        }

        prefab_id prefab_from_file(std::string_view file) const {
            const auto json = nlohmann::json::parse(std::ifstream(file.data()));
            const auto components = json["components"];

            std::vector<instruction> instructions;

            // get all components described in json
            for (const auto& [name, data] : components.items()) {
                ASSERT(m_components.contains(name), "Json component not found!");

                const auto& info = m_components.at(name);
                
                // store instructions for execution later
                instructions.emplace_back(info(data, c_context));
            }

            // create and return a prefab handle
            return m_prefab_manager.create(std::move(instructions));
        }

    private:
        entt::registry& m_registry;
        prefab_manager& m_prefab_manager;
        const asset_context& c_context;

        std::unordered_map<std::string_view, component_info> m_components;
    };
}
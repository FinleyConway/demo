#pragma once

#include <string>
#include <fstream>
#include <functional>
#include <string_view>
#include <unordered_map>

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "core/debugging.hpp"
#include "ecs/core/prefab.hpp"
#include "ecs/core/prefab_manager.hpp"
#include "resources/asset_context.hpp"

namespace demo::ecs {
    template<typename T>
    concept json_serializable = requires(nlohmann::json& j, const nlohmann::json& cj, T& value, const T& cvalue) {
        to_json(j, cvalue);
        from_json(cj, value);
    };

    class prefab_finder {
    public:
        explicit prefab_finder(prefab_manager& prefab_manager) :
            m_prefab_manager(prefab_manager)
        {
        }

        std::pair<prefab_id, bool> get(const std::string& name) {
            const auto it = m_prefab_lookup.find(name);
            
            if (it == m_prefab_lookup.end()) {
                const prefab_id id = m_prefab_manager.reserve_prefab();

                m_prefab_lookup.emplace(name, id);

                return { id, true };
            }

            return { it->second, false };
        }

    private:
        prefab_manager& m_prefab_manager;
        std::unordered_map<std::string, prefab_id> m_prefab_lookup;
    };

    class entity_loader {
    private:
        using instruction = std::function<void(entt::registry&, entt::entity)>;
        using component_info = std::function<instruction(const nlohmann::json&, const asset_context&)>;

    public:
        entity_loader(entt::registry& registry, prefab_manager& prefab_manager, const asset_context& context) :
            m_registry(registry),
            m_prefab_manager(prefab_manager),
            c_context(context),
            m_prefab_finder(m_prefab_manager)
        {
        }    

        template<typename T>
        void register_component() {
            constexpr std::string_view name = get_component_name<T>();

            m_components.try_emplace(
                name, 
                [](const nlohmann::json& json, const asset_context&) {
                    if constexpr (json_serializable<T>) {
                        return [component = json.get<T>()](entt::registry& r, entt::entity e) {
                            r.emplace<T>(e, component);
                        };
                    } 
                    else {
                        return [](entt::registry& r, entt::entity e) {
                            r.emplace<T>(e);
                        };
                    }
                }
            );
        }

        template<typename T, typename Fn>
        void register_component(Fn&& deserialise) {
            constexpr std::string_view name = get_component_name<T>();

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

        prefab_id prefab_from_file(std::string_view file) {
            const auto json = nlohmann::json::parse(std::ifstream(file.data()));

            const auto prefab_name = json["name"];
            const auto components = json["components"];

            std::vector<instruction> instructions;

            // get all components described in json
            for (const auto& [name, data] : components.items()) {
                ASSERT(m_components.contains(name), "Json component not found!");

                const auto& info = m_components.at(name);
                
                // store instructions for execution later
                instructions.emplace_back(info(data, c_context));
            }

            // get id
            auto [id, created] = m_prefab_finder.get(prefab_name);

            // has 
            if (!created) {
                m_prefab_manager.update_reserved(id, std::move(instructions));
            }
            
            // return a prefab handle
            return id;
        }

    private:
        template<typename T>
        consteval std::string_view get_component_name() {
            constexpr std::string_view name = entt::type_name<T>::value();
            constexpr std::string_view prefix = "demo::ecs::";

            return name.starts_with(prefix)
                ? name.substr(prefix.size())
                : name;
        }

    private:
        entt::registry& m_registry;
        prefab_manager& m_prefab_manager;
        const asset_context& c_context;

        prefab_finder m_prefab_finder;
        std::unordered_map<std::string_view, component_info> m_components;
    };
}
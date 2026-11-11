#pragma once

#include <functional>
#include <string_view>
#include <unordered_map>

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "resources/asset_context.hpp"

namespace demo::ecs {
    class prefab_finder; // forward declaration

    template<typename T>
    concept json_serializable = requires(nlohmann::json& j, const nlohmann::json& cj, T& value, const T& cvalue) {
        to_json(j, cvalue);
        from_json(cj, value);
    };

    class component_registry {
    public:
        using instruction = std::function<void(entt::registry&, entt::entity)>;

        using component_info = std::function<
            instruction(const nlohmann::json&, const asset_context&, prefab_finder&)
        >;
    
    public:
        template<typename T>
        void register_component() {
            constexpr std::string_view name = get_component_name<T>();

            m_components.try_emplace(
                name, 
                [](const nlohmann::json& json, const asset_context&, prefab_finder&) {
                    // if the component has adl json functions
                    if constexpr (json_serializable<T>) {
                        // create an instruction copying the json data into it
                        return [component = json.get<T>()](entt::registry& r, entt::entity e) {
                            r.emplace<T>(e, component);
                        };
                    } 
                    // if the component is empty or doesnt need serialisation
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
                [fn = std::forward<Fn>(deserialise)]
                (const nlohmann::json& json, const asset_context& context, prefab_finder& finder) {
                    T component = fn(json, context, finder);

                    return [component = std::move(component)](entt::registry& r, entt::entity e) {
                        r.emplace<T>(e, component);
                    };
                }
            );
        }

        bool has_component_info(std::string_view component_name) const {
            return m_components.contains(component_name);
        }

        const component_info& get_component_info(std::string_view component_name) const {
            return m_components.at(component_name);
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
        std::unordered_map<std::string_view, component_info> m_components;
    };
}
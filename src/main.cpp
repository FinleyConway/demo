#include <fstream>
#include <iostream>

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "core/debugging.hpp"
#include "entt/entity/fwd.hpp"
#include "resources/asset_context.hpp"

using namespace demo;

class component_registry {
private:
    using instruction = std::function<void(entt::registry&, entt::entity)>;
    using component_info = std::function<instruction(const nlohmann::json&, const asset_context&)>;

public:
    explicit component_registry(entt::registry& registry, const asset_context& context) :
        m_registry(registry),
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

    std::vector<instruction> prefab_from_file(std::string_view file) const {
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

        return instructions;
    }

private:
    entt::registry& m_registry;
    const asset_context& c_context;

    std::unordered_map<std::string_view, component_info> m_components;
};

struct position {
    float x = 0;
    float y = 0;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(position, x, y);
};

struct rotation {
    float angle = 0;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(rotation, angle);
};

struct scale {
    float x = 0;
    float y = 0;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(scale, x, y);
};

int main() {
    entt::registry registry;
    asset_context context;
    component_registry reg(registry, context);

    reg.register_component<position>();
    reg.register_component<rotation>();
    reg.register_component<scale>();

    auto instructions = reg.prefab_from_file("/var/home/finley/Projects/Git/demo/test_assets/entities/test_entity.json");

    auto e = registry.create();

    for (auto i : instructions) {
        i(registry, e);
    }

    auto& p = registry.get<position>(e);
    auto& r = registry.get<rotation>(e);
    auto& s = registry.get<scale>(e);

    std::cout << p.x << " " << p.y << std::endl;
    std::cout << r.angle << std::endl;
    std::cout << s.x << " " << s.y << std::endl;
}
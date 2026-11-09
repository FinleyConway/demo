#include <fstream>
#include <iostream>

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "core/debugging.hpp"
#include "resources/asset_context.hpp"

using namespace demo;

class component_registry {
private:
    using component_info = void(*)(entt::registry&, entt::entity, const nlohmann::json&);
    using prefab_instruction = void(*)(entt::registry&, entt::entity, const asset_context&);

public:
    explicit component_registry(entt::registry& registry) :
        m_registry(registry)
    {
    }    

    template<typename T>
    void register_component() {
        constexpr std::string_view name = entt::type_name<T>::value();

        m_components.try_emplace(name, [](entt::registry& r, entt::entity e, const nlohmann::json& j) {
            r.emplace<T>(e, j.get<T>());
        });
    }

    entt::entity entity_from_file(std::string_view file) const {
        const auto entity = m_registry.create();

        const auto json = nlohmann::json::parse(std::ifstream(file.data()));
        const auto components = json["components"];

        for (const auto& [name, data] : components.items()) {
            ASSERT(m_components.contains(name), "Json component not found!");

            const auto& info = m_components.at(name);

            info(m_registry, entity, data);
        }

        return entity;
    }

private:
    entt::registry& m_registry;
    std::unordered_map<std::string_view, component_info> m_components;
};

struct position {
    float x = 0;
    float y = 0;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(position, x, y);
};

int main() {
    entt::registry r;
    component_registry reg(r);

    reg.register_component<position>();

    auto e = reg.entity_from_file("/var/home/finley/Projects/Git/demo/test_assets/entities/test_entity.json");

    auto& p = r.get<position>(e);

    std::cout << p.x << " " << p.y << std::endl;
}
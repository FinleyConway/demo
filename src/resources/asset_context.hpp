#pragma once

#include <chrono>
#include <limits>
#include <memory>
#include <vector>
#include <cstddef>
#include <filesystem>
#include <unordered_map>

#include "core/debugging.hpp"
#include "resources/resource_base.hpp"
#include "resources/types/texture.hpp"
#include "resources/types/animation_clip.hpp"

namespace demo {
    using asset_id = size_t;
    static constexpr asset_id invalid_asset_id = std::numeric_limits<asset_id>::max();

    template<typename T>
    struct resource_traits;

    template<>
    struct resource_traits<texture> {
        static constexpr resource_type type = resource_type::texture;
    };

    template<>
    struct resource_traits<animation_clip> {
        static constexpr resource_type type = resource_type::animation_clip;
    };

    class asset_context {
    public:
        asset_context() = default;

        asset_context(const asset_context&) = delete;
        asset_context& operator=(const asset_context&) = delete;

        asset_context(asset_context&&) noexcept = default;
        asset_context& operator=(asset_context&&) noexcept = default;
    
    public:
        void load_from_dir(std::string_view path) {
            using namespace std::filesystem;

            // load prefabs first
            for (const auto& dir_entry : recursive_directory_iterator(path)) {
                asset_from_file(dir_entry);
            }
        }

        asset_id find(std::string_view name) const {
            auto it = m_lookup.find(std::string(name));

            ASSERT(it != m_lookup.end(), "Asset not found!");

            return it->second;
        }

        template<typename T>
        const T& get(asset_id id) const {
            ASSERT(id < m_resources.size(), "Invalid asset_id!");

            const auto& resource = m_resources[id];

            ASSERT(
                resource->type() == resource_traits<T>::type,
                "Asset type mismatch!"
            );

            return static_cast<const T&>(*resource); 
        }

    private:
        void asset_from_file(const std::filesystem::path& path) {
            LOG_INFO("ASSET: loading %s", path.c_str());

            const auto json = nlohmann::json::parse(std::ifstream(path));
            const auto type = json["resource_type"].get<resource_type>();

            ASSERT(type != resource_type::none, "Trying to load a none asset type");

            if (type == resource_type::texture) load_texture(json);
            if (type == resource_type::animation_clip) load_animation_clip(json);
        }

        void load_texture(const nlohmann::json& json) {
            const auto name = json["name"];
            const auto path = json["file_path"].get<std::string_view>();

            add<texture>(name, path);
        }

        void load_animation_clip(const nlohmann::json& json) {
            const auto name = json["name"];
            const auto loop_type = json["loop_mode"].get<loop_mode>();

            std::vector<animation_frame> frames;

            if (json.contains("frame_count")) {
                const auto frame_count = json["frame_count"];
                const auto source = json["source"].get<rect_int>();
                const auto duration = json["duration_ms"].get<std::chrono::milliseconds>();

                for (auto i = 0; i < frame_count; i++) {
                    frames.emplace_back(
                        rect_int {
                            .x = source.x + source.w * i,
                            .y = source.h,
                            .w = source.w,
                            .h = source.h
                        }, 
                        duration
                    );
                }
            }
            // do array of frames later

            add<animation_clip>(name, animation_clip { std::move(frames), loop_type });
        }

        template<typename T, typename... Args>
        void add(std::string&& name, Args... args) {
            const asset_id id = m_resources.size();

            m_resources.emplace_back(
                std::make_unique<T>(std::forward<Args>(args)...)
            );
            m_lookup.emplace(std::move(name), id);
        }

    private:
        std::unordered_map<std::string, asset_id> m_lookup;
        std::vector<std::unique_ptr<resource_base>> m_resources;
    };
}
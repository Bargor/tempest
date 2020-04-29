// This file is part of Tempest-application project
// Author: Karol Kontny

#include "data_loader.h"

#include "data_exception.h"

#include <fmt/format.h>
#include <fmt/printf.h>
#include <fstream>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/rapidjson.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace tst {
namespace application {

    data_loader::data_loader() noexcept {
        m_searchPaths.emplace_back("./");
    }

    data_loader::data_loader(std::initializer_list<std::filesystem::path> pathList) noexcept : m_searchPaths(pathList) {
    }

    data_loader::~data_loader() {
    }

    std::string data_loader::load_text_file(const std::filesystem::path& path) const {
        std::ifstream input(path, std::ios::in);
        if (input) {
            std::string contents;
            input.seekg(0, std::ios::end);
            contents.resize(input.tellg());
            input.seekg(0, std::ios::beg);
            input.read(&contents[0], contents.size());
            input.close();
            return (contents);
        }
        throw data_exception(fmt::sprintf("Can't load file: %s", path.string()));
    }

    std::vector<char> data_loader::load_shader_bytecode(const std::filesystem::path& path) const {
        std::ifstream input(path, std::ios::in | std::ios::binary);
        if (input) {
            std::vector<char> contents;
            input.seekg(0, std::ios::end);
            contents.resize(input.tellg());
            input.seekg(0, std::ios::beg);
            input.read(&contents[0], contents.size());
            input.close();
            return (contents);
        }
        throw data_exception(fmt::sprintf("Can't load file: %s", path.string()));
    }

    void data_loader::load_obj_model(const std::string_view&) const {
    }

    rapidjson::Document data_loader::load_json(const std::filesystem::path& path) const {
        std::ifstream input(path, std::ios::in);
        rapidjson::IStreamWrapper inputStreamWrapper(input);

        rapidjson::Document jsonDocument;
        jsonDocument.ParseStream(inputStreamWrapper);
        return jsonDocument;
    }

    image_data data_loader::load_image(const std::filesystem::path& path) const {
        std::int32_t texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load(path.string().c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        return {{static_cast<std::uint32_t>(texWidth), static_cast<std::uint32_t>(texHeight)},
                texChannels,
                static_cast<std::size_t>(texWidth) * texHeight * STBI_rgb_alpha,
                ptr<unsigned char, void (*)(void*)>(pixels, stbi_image_free)};
    }

    std::optional<std::filesystem::path> data_loader::find_file(const std::string& name) const {
        return find_file(std::filesystem::path(name));
    }

    std::optional<std::filesystem::path> data_loader::find_file(const std::filesystem::path& name) const {
        for (const auto& path : m_searchPaths) {
            const auto filename = path / name;
            if (std::filesystem::exists(filename)) {
                return filename;
            }
        }
        return std::nullopt;
    }

    void data_loader::add_search_path(std::string&& path) noexcept {
        m_searchPaths.emplace_back(std::move(path));
    }

} // namespace application
} // namespace tst
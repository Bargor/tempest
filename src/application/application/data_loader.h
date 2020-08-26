// This file is part of Tempest-application project
// Author: Karol Kontny
#pragma once

#include "image_data.h"

#include <filesystem>
#include <optional>
#include <rapidjson/document.h>
#include <string_view>
#include <vector>

namespace tst {
namespace application {

    class data_loader {
    public:
        data_loader() noexcept;
        data_loader(std::initializer_list<std::filesystem::path> pathList) noexcept;

        virtual ~data_loader();

    public:
        virtual std::string load_text_file(const std::filesystem::path& path) const;
        virtual std::vector<char> load_shader_bytecode(const std::filesystem::path& path) const;
        virtual void load_obj_model(const std::string_view& name) const;
        virtual rapidjson::Document load_json(const std::filesystem::path& path) const;
        virtual image_data load_image(const std::filesystem::path& path) const;
        virtual std::optional<std::filesystem::path> find_file(const std::string_view name) const;
        virtual std::optional<std::filesystem::path> find_file(const std::filesystem::path& name) const;

    public:
        void add_search_path(std::string path) noexcept;

    private:
        std::vector<std::filesystem::path> m_searchPaths;
    };

} // namespace application
} // namespace tst
// This file is part of Tempest-application project
// Author: Karol Kontny
#pragma once

#include <filesystem>
#include <string_view>
#include <vector>

namespace tst {
namespace application {

    class data_loader {
    public:
        data_loader();
        data_loader(std::initializer_list<std::filesystem::path> pathList);

        virtual ~data_loader();

    public:
        virtual std::string load_shader_source(const std::string_view& name, const std::string_view& format) const;
        virtual void load_obj_model(const std::string_view& name) const;

    public:
        void add_search_path(std::string&& path) noexcept;

    private:
        std::vector<std::filesystem::path> m_searchPaths;
    };

} // namespace application
} // namespace tst
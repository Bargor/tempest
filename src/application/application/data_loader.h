// This file is part of Tempest-application project
// Author: Karol Kontny
#pragma once

#include <filesystem>
#include <string_view>
#include <vector>
#include <optional>

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
        virtual std::optional<std::filesystem::path> find_file(const std::string& name) const;
        virtual std::optional<std::filesystem::path> find_file(const std::filesystem::path& name) const;

    public:
        void add_search_path(std::string&& path) noexcept;

	public:
#ifdef _WIN32
        static constexpr char separator = '\\';
#else
        static constexpr char separator = '/';
#endif

    private:
        std::vector<std::filesystem::path> m_searchPaths;
    };

} // namespace application
} // namespace tst
// This file is part of Tempest-application project
// Author: Karol Kontny

#include "data_loader.h"

#include "data_exception.h"

namespace tst {
namespace application {

    data_loader::data_loader() { m_searchPaths.emplace_back("/"); }

    data_loader::data_loader(std::initializer_list<std::filesystem::path> pathList) : m_searchPaths(pathList) {}

    data_loader::~data_loader() {}

    std::string data_loader::load_shader_source(const std::string_view&, const std::string_view&) const {
        return std::string{};
    }

    void data_loader::load_obj_model(const std::string_view&) const {
    }

    void data_loader::add_search_path(std::string&& path) noexcept { m_searchPaths.emplace_back(std::move(path)); }

} // namespace application
} // namespace tst
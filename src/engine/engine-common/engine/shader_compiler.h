// This file is part of Tempest-engine-GL project
// Author: Karol Kontny

#pragma once

#include <backend/shader.h>

#include <optional>

namespace tst {
namespace application {
    class data_loader;
}

namespace engine {

    class shader_compiler {
    public:
        std::optional<opengl::shader_program> compile_program(std::string_view&& name,
                                                              const opengl::shaderTypesSet& types) const;

    public:
        shader_compiler(application::data_loader& data_loader);

    private:
        std::optional<opengl::shader> compile_shader(const std::string_view name, const opengl::shader_type type) const;

    private:
        application::data_loader& m_dataLoader;
    };

} // namespace engine
} // namespace tst
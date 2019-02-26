// This file is part of Tempest-engine-GL project
// Author: Karol Kontny

#include "shader_compiler.h"

#include <backend/opengl_exception.h>

#include <algorithm>
#include <application/data_exception.h>
#include <application/data_loader.h>
#include <fmt/printf.h>

namespace tst {
namespace engine {

    shader_compiler::shader_compiler(application::data_loader& data_loader) : m_dataLoader(data_loader) {}

    std::optional<opengl::shader_program> shader_compiler::compile_program(std::string_view&& name,
                                                                           const opengl::shaderTypesSet& types) const {
        std::array<std::optional<opengl::shader>, opengl::shader_types_count> shaders{std::nullopt};
        for (std::size_t i = 0; i < shaders.size(); i++) {
            if (types[i]) {
                shaders[i] = compile_shader(name, static_cast<opengl::shader_type>(i));
                if (!shaders[i]) {
                    return std::nullopt;
                }
            }
        }
        opengl::shader_program program(name);
        std::for_each(shaders.begin(), shaders.end(), [&](auto& shader) {
            if (shader) {
                program.attach_shader(shader.value());
            }
        });
        if (program.link()) {
            return program;
        }
        return std::nullopt;
    } 

    std::string get_shader_format(const opengl::shader_type type) {
        std::array<std::string, static_cast<std::size_t>(opengl::shader_type::enum_size)> formats = {
            "vs", "fs", "gs", "cs", "", ""};
        return formats[static_cast<std::size_t>(type)];
    }

    std::optional<opengl::shader> shader_compiler::compile_shader(const std::string_view name, const opengl::shader_type type) const {

        try {
            auto shader = opengl::shader(type, m_dataLoader.load_shader_source(name, get_shader_format(type)), name);
            if (shader.compile()) {
                return shader;
            }
        } catch (opengl::opengl_exception&) {
        } catch (application::data_exception&) {
        }
        return std::nullopt;
    }

} // namespace engine
} // namespace tst
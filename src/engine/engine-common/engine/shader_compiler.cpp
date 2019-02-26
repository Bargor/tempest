// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "shader_compiler.h"

#include <backend/api_exception.h>

#include <algorithm>
#include <application/data_exception.h>
#include <application/data_loader.h>
#include <fmt/printf.h>

namespace tst {
namespace engine {

    shader_compiler::shader_compiler(application::data_loader& data_loader) : m_dataLoader(data_loader) {}

    std::optional<backend::shader_program> shader_compiler::compile_program(std::string_view&& name,
                                                                            const backend::shaderTypesSet& types) const {
        std::array<std::optional<backend::shader>, backend::shader_types_count> shaders{std::nullopt};
        for (std::size_t i = 0; i < shaders.size(); i++) {
            if (types[i]) {
                shaders[i] = compile_shader(name, static_cast<backend::shader_type>(i));
                if (!shaders[i]) {
                    return std::nullopt;
                }
            }
        }
        backend::shader_program program(name);
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

    std::string get_shader_format(const backend::shader_type type) {
        std::array<std::string, static_cast<std::size_t>(backend::shader_type::enum_size)> formats = {
            "vs", "fs", "gs", "cs", "", ""};
        return formats[static_cast<std::size_t>(type)];
    }

    std::optional<backend::shader> shader_compiler::compile_shader(const std::string_view name,
                                                                  const backend::shader_type type) const {

        try {
            auto shader = backend::shader(type, m_dataLoader.load_shader_source(name, get_shader_format(type)), name);
            if (shader.compile()) {
                return shader;
            }
        } catch (backend::api_exception&) {
        } catch (application::data_exception&) {
        }
        return std::nullopt;
    }

} // namespace engine
} // namespace tst
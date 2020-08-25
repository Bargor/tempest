// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "shader_compiler.h"

#include "opengl_exception.h"

#include <algorithm>
#include <application/data_exception.h>
#include <application/data_loader.h>
#include <fmt/printf.h>

namespace tst {
namespace engine {
    namespace opengl {

        shader_compiler::shader_compiler(const application::data_loader& data_loader) : m_dataLoader(data_loader) {
        }

        std::optional<shader_program> shader_compiler::compile_program(std::string_view&& name,
                                                                       const shaderTypesSet& types) const {
            std::array<std::optional<shader>, base::shader_types_count> shaders{std::nullopt};
            for (std::size_t i = 0; i < shaders.size(); i++) {
                if (types[i]) {
                    shaders[i] = compile_shader(std::string(name), static_cast<shader_type>(i));
                    if (!shaders[i]) {
                        return std::nullopt;
                    }
                }
            }
            shader_program program(name);
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

        std::string get_shader_format(const shader_type type) {
            static std::array<std::string, static_cast<std::size_t>(shader_type::enum_size)> formats = {
                ".vs", ".fs", ".gs", ".cs", "", ""};
            return formats[static_cast<std::size_t>(type)];
        }

        std::optional<shader> shader_compiler::compile_shader(const std::string& name, const shader_type type) const {
            try {
                auto shader = opengl::shader(
                    type, m_dataLoader.load_text_file(std::filesystem::path(name + get_shader_format(type))), name);
                if (shader.compile()) {
                    return shader;
                }
            } catch (opengl_exception&) {
            } catch (application::data_exception&) {
            }
            return std::nullopt;
        }

    } // namespace opengl
} // namespace engine
} // namespace tst
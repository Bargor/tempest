// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "shader_compiler.h"

#include "device.h"

#include <application/data_loader.h>
#include <fmt/printf.h>

namespace tst {
namespace engine {
    namespace vulkan {

        std::string get_shader_format(const shader::shader_type type) {
            static std::array<std::string, static_cast<std::size_t>(shader::shader_type::enum_size)> formats = {
                "vert", "frag", "geom", "comp", "tesc", "tese"};
            return formats[static_cast<std::size_t>(type)];
        }

        bool compile_shader(const std::string& shaderBytecodeFile, const std::string shaderSourceFile) {
            std::string command = fmt::sprintf("glslangValidator -V -o %s %s", shaderBytecodeFile, shaderSourceFile);
            return std::system(command.c_str());
        }

        shader_compiler::shader_compiler(application::data_loader& data_loader, const device& device)
            : m_dataLoader(data_loader), m_device(device.m_logicalDevice) {
        }

        shader_compiler::~shader_compiler() {
        }

        shader_set shader_compiler::compile_shaders(const std::string& name) const {
            shader_set shaders;

            for (std::int32_t idx = 0; idx < static_cast<std::int32_t>(shader::shader_type::enum_size); ++idx) {
                std::string shaderFileName(std::string("shaders") + application::data_loader::separator + name + "." +
                                           get_shader_format(static_cast<shader::shader_type>(idx)));
                std::string bytecodeFileName(name + "." + get_shader_format(static_cast<shader::shader_type>(idx)) +
                                             m_shaderExtension);

                auto shaderSourceFile = m_dataLoader.find_file(shaderFileName);
                auto shaderBytecodeFile = m_dataLoader.find_file(bytecodeFileName);

                auto bytecode = load_bytecode(shaderSourceFile, shaderBytecodeFile, bytecodeFileName);

                if (!bytecode) {
                    continue;
                }

                shader shaderModule(m_device, static_cast<shader::shader_type>(idx), std::move(bytecode.value()), name);

                shaders.emplace_back(std::move(shaderModule));
            }
            return shaders;
        }

        std::optional<std::vector<char>>
        shader_compiler::load_bytecode(const std::optional<std::filesystem::path>& shaderSourceFile,
                                       const std::optional<std::filesystem::path>& shaderBytecodeFile,
                                       const std::string& bytecodeFileName) const {
            if (shaderSourceFile) {
                if (shaderBytecodeFile) {
                    auto sourceWriteTime = std::filesystem::last_write_time(shaderSourceFile.value());
                    auto bytecodeWriteTime = std::filesystem::last_write_time(shaderBytecodeFile.value());

                    if (sourceWriteTime > bytecodeWriteTime) { // we have to compile shader
                        if (!compile_shader(bytecodeFileName, shaderSourceFile.value().string())) {
                            return std::nullopt;
                        }
                    }
                } else { // only source file is available
                    if (!compile_shader(bytecodeFileName, shaderSourceFile.value().string())) {
                        return std::nullopt;
                    }
                }
            } else {
                if (!shaderBytecodeFile) { // none of the files exist
                    return std::nullopt;
                }
            }

            return m_dataLoader.load_shader_bytecode(shaderBytecodeFile.value());
        }

    } // namespace vulkan

} // namespace engine
} // namespace tst
// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "shader_compiler.h"

#include "shader.h"

#include <application/data_exception.h>
#include <application/data_loader.h>

namespace tst {
namespace engine {
    namespace vulkan {

        std::string get_shader_format(const shader::shader_type type) {
            static std::array<std::string, static_cast<std::size_t>(shader::shader_type::enum_size)> formats = {
                "vert", "frag", "geom", "comp", "tesc", "tese"};
            return formats[static_cast<std::size_t>(type)];
        }


        shader_compiler::shader_compiler(application::data_loader& data_loader, const vk::Device& device)
            : m_dataLoader(data_loader), m_device(device) {
        }

        shader_set shader_compiler::compile_shaders(const std::string& name) const {

            shader_set shaders;

            for (std::int32_t idx = 0; idx < static_cast<std::int32_t>(shader::shader_type::enum_size); ++idx) {
                auto shaderSourceFile =
                    m_dataLoader.find_file((name + "." + get_shader_format(static_cast<shader::shader_type>(idx))));

                auto shaderBytecodeFile = m_dataLoader.find_file(
                    name + "." + get_shader_format(static_cast<shader::shader_type>(idx)) + m_shaderExtension);

                if (shaderSourceFile && shaderBytecodeFile) {
                    auto sourceWriteTime = std::filesystem::last_write_time(shaderSourceFile.value());
                    auto bytecodeWriteTime = std::filesystem::last_write_time(shaderBytecodeFile.value());

                    if (sourceWriteTime > bytecodeWriteTime) { // we have to compile shader
                    }
                    auto bytecode = m_dataLoader.load_shader_bytecode(name);

                    auto shaderModule =
                        shader(m_device, static_cast<shader::shader_type>(idx), std::move(bytecode), name);

                    shaders.emplace_back(shaderModule.get_create_info());
                }
            }
            return shaders;
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "shader_compiler.h"

#include "device.h"
#include "resource_cache.h"

#include <application/data_loader.h>
#include <fmt/printf.h>

namespace tst {
namespace engine {
    namespace vulkan {

        std::string get_shader_format(const shader_type type) {
            static std::array<std::string, static_cast<std::size_t>(shader_type::enum_size)> formats = {
                "vert", "frag", "geom", "comp", "tesc", "tese"};
            return formats[static_cast<std::size_t>(type)];
        }

        bool compile_shader(const std::string& shaderBytecodeFile, const std::string& shaderSourceFile) {
            std::string command = fmt::sprintf("glslangValidator -V -o %s %s", shaderBytecodeFile, shaderSourceFile);
            return std::system(command.c_str());
        }

        std::vector<shader::descriptor_layout> parse_descriptor_layouts(const rapidjson::Document& jsonModel,
                                                                        shader_type type) {
            assert(jsonModel.HasMember(get_shader_format(type).c_str()));
            const auto& descriptors = jsonModel[get_shader_format(type).c_str()];
            assert(descriptors.IsArray());
            std::vector<shader::descriptor_layout> layouts;
            layouts.reserve(descriptors.GetArray().Size());

            for (const auto& descriptor : descriptors.GetArray()) {
                auto binding = descriptor["binding"].GetUint();

                layouts.emplace_back(shader::descriptor_layout{
                    binding, vk::DescriptorType::eUniformBuffer, shader::get_native_shader_type(type)});
            }
            return layouts;
        }

        shader_compiler::shader_compiler(const application::data_loader& dataLoader,
                                         const vk::Device device,
                                         resource_cache& resourceCache)
            : m_dataLoader(dataLoader), m_device(device), m_resourceCache(resourceCache) {
        }

        shader_compiler::~shader_compiler() {
        }

        shader_set shader_compiler::compile_shaders(const std::string& name) const {
            shader_set shaders;

            auto jsonDescriptorFile = m_dataLoader.find_file(std::filesystem::path("shaders") / (name + ".json"));
            assert(jsonDescriptorFile);
            const auto& jsonModel = m_dataLoader.load_json(jsonDescriptorFile.value());

            for (std::int32_t idx = 0; idx < static_cast<std::int32_t>(shader_type::enum_size); ++idx) {
                const auto shaderFileName(std::filesystem::path("shaders") /
                                           (name + "." + get_shader_format(static_cast<shader_type>(idx))));
                const auto bytecodeFileName(name + "." + get_shader_format(static_cast<shader_type>(idx)) +
                                             m_shaderExtension);

                const auto shaderSourceFile = m_dataLoader.find_file(shaderFileName);
                const auto shaderBytecodeFile = m_dataLoader.find_file(bytecodeFileName);

                auto bytecode = load_bytecode(shaderSourceFile, shaderBytecodeFile, bytecodeFileName);

                if (!bytecode) {
                    continue;
                }

                auto descriptorLayouts = parse_descriptor_layouts(jsonModel, static_cast<shader_type>(idx));

                for (auto& layout : descriptorLayouts) {
                    const vk::DescriptorSetLayoutBinding descriptorBinding(layout.binding, layout.type, 1, layout.stages, nullptr);

                    vk::DescriptorSetLayoutCreateInfo setLayoutInfo(
                        vk::DescriptorSetLayoutCreateFlags(), 1, &descriptorBinding);

                    const auto vkLayout = m_device.createDescriptorSetLayout(setLayoutInfo, nullptr);
                    m_resourceCache.add_descritptor_set_layout(std::move(layout), vkLayout);
                }
                shader shaderModule(m_device,
                                    static_cast<shader_type>(idx),
                                    std::move(bytecode.value()),
                                    name,
                                    std::move(descriptorLayouts));

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
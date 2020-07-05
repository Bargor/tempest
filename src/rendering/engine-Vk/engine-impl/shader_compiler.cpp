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

        std::vector<vk::DescriptorSetLayoutBinding> parse_descriptor_layouts_binding(const rapidjson::Value& set) {
            std::vector<vk::DescriptorSetLayoutBinding> bindings;

            for (std::int32_t idx = 0; idx < static_cast<std::int32_t>(shader_type::enum_size); ++idx) {
                if (!set.HasMember(get_shader_format(static_cast<shader_type>(idx)).c_str())) {
                    continue;
                }
                const auto& descriptors = set[get_shader_format(static_cast<shader_type>(idx)).c_str()];

                for (const auto& descriptor : descriptors.GetArray()) {
                    const auto binding = descriptor["binding"].GetUint();
                    const auto descriptorType = descriptor["type"].GetUint();
                    bindings.emplace_back(binding,
                                          static_cast<vk::DescriptorType>(descriptorType),
                                          1,
                                          shader::get_native_shader_type(static_cast<shader_type>(idx)),
                                          nullptr);
                }
            }
            return bindings;
        }

        shader_compiler::shader_compiler(const application::data_loader& dataLoader,
                                         const vk::Device device)
            : m_dataLoader(dataLoader), m_device(device) {
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

                auto bytecode = load_bytecode(
                    m_dataLoader.find_file(shaderFileName), m_dataLoader.find_file(bytecodeFileName), bytecodeFileName);

                if (!bytecode) {
                    continue;
                }

                shader shaderModule(m_device, static_cast<shader_type>(idx), std::move(bytecode.value()), name);

                shaders.shaders.emplace_back(std::move(shaderModule));
            }

            std::tie(shaders.layouts, shaders.bindings) = create_descriptor_layouts(jsonModel);

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

        std::tuple<std::vector<vk::DescriptorSetLayout>, std::vector<std::vector<vk::DescriptorSetLayoutBinding>>>
        shader_compiler::create_descriptor_layouts(const rapidjson::Document& jsonModel) const {
            const auto& sets = jsonModel["sets"];
            assert(sets.IsArray());
            std::vector<vk::DescriptorSetLayout> layouts;
            std::vector<std::vector<vk::DescriptorSetLayoutBinding>> bindings;
            layouts.reserve(sets.GetArray().Size());
            bindings.reserve(sets.GetArray().Size());

            for (const auto& set : sets.GetArray()) {
                auto descriptorLayoutsBindings = parse_descriptor_layouts_binding(set);

                vk::DescriptorSetLayoutCreateInfo setLayoutInfo(
                    vk::DescriptorSetLayoutCreateFlags(),
                    static_cast<std::uint32_t>(descriptorLayoutsBindings.size()),
                    descriptorLayoutsBindings.data());

                const auto vkLayout = m_device.createDescriptorSetLayout(setLayoutInfo, nullptr);

                layouts.emplace_back(vkLayout);
                bindings.emplace_back(std::move(descriptorLayoutsBindings));
            }

            return std::tie(layouts, bindings);
        }

    } // namespace vulkan

} // namespace engine
} // namespace tst
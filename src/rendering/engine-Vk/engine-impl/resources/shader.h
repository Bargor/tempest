// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-base/shader.h>

#include <string>
#include <util/hash.h>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {

        using shader_type = base::shader_type;

        class shader {
        public:
            static vk::ShaderStageFlagBits get_native_shader_type(shader_type type);

            struct descriptor_layout {
                std::uint32_t binding;
                vk::DescriptorType type;
                vk::ShaderStageFlagBits stages;

                bool operator==(const descriptor_layout& other) const noexcept;
            };

        public:
            shader(vk::Device device,
                   shader_type type,
                   std::vector<char>&& source,
                   const std::string& name,
                   std::vector<shader::descriptor_layout>&& layouts);
            shader(const shader&) = delete;
            shader(shader&& shader) noexcept;
            ~shader();

        public:
            shader& operator=(const shader&) = delete;

        public:
            vk::PipelineShaderStageCreateInfo get_pipeline_info() const noexcept;
            const std::vector<shader::descriptor_layout>& get_layouts() const noexcept;
            shader_type get_stage() const noexcept;

        private:
            vk::Device m_device;
            std::vector<char> m_source;
            std::string m_name;
            shader_type m_type;
            vk::ShaderModule m_shader;
            vk::PipelineShaderStageCreateInfo m_pipelineInfo;
            std::vector<descriptor_layout> m_descriptorLayouts;
        };

        using shader_set = std::vector<shader>;
    } // namespace vulkan
} // namespace engine
} // namespace tst

namespace std {

template<>
struct hash<tst::engine::vulkan::shader::descriptor_layout> {
    std::size_t operator()(const tst::engine::vulkan::shader::descriptor_layout& layout) const {
        size_t seed = 0;

        tst::hash_combine(seed, std::hash<std::uint32_t>{}(layout.binding));
        tst::hash_combine(seed, std::hash<std::uint32_t>{}(static_cast<std::uint32_t>(layout.type)));
        tst::hash_combine(seed, std::hash<std::uint32_t>{}(static_cast<std::uint32_t>(layout.stages)));

        return seed;
    }
};

} // namespace std

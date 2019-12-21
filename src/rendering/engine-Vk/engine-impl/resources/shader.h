// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-base/shader.h>

#include <string>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {

        using shader_type = base::shader_type;

        class shader {
        public:

            static vk::ShaderStageFlagBits get_native_shader_type(shader_type type);

        public:
            shader(vk::Device device,
                   shader_type type,
                   std::vector<char>&& source,
                   const std::string& name,
                   std::vector<vk::DescriptorSetLayout>&& layouts);
            shader(const shader&) = delete;
            shader(shader&& shader) noexcept;
            ~shader();

        public:
            shader& operator=(const shader&) = delete;

        public:
            vk::PipelineShaderStageCreateInfo get_pipeline_info() const noexcept;
            const std::vector<vk::DescriptorSetLayout>& get_layouts() const noexcept;
            shader_type get_stage() const noexcept;

        private:
            vk::Device m_device;
            std::vector<char> m_source;
            std::string m_name;
            shader_type m_type;
            vk::ShaderModule m_shader;
            vk::PipelineShaderStageCreateInfo m_pipelineInfo;
            std::vector<vk::DescriptorSetLayout> m_descriptorLayouts;
        };

        using shader_set = std::vector<shader>;
    } // namespace vulkan
} // namespace engine
} // namespace tst

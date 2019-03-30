// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {

        class shader {
        public:
            enum class shader_type : std::uint8_t {
                vertex,
                fragment,
                geometry,
                compute,
                tessellation_control,
                tessellation_evaluation,
                enum_size = 6
            };

        public:
            shader(vk::Device device, shader_type type, std::vector<char>&& source, const std::string_view name);
            shader(const shader&) = delete;
            shader(shader&& shader);
            ~shader();

        public:
            shader& operator=(const shader&) = delete;
            shader& operator=(shader&& rhs);

        public:
            vk::PipelineShaderStageCreateInfo get_create_info() const;

        private:
            vk::Device& m_device;
            std::vector<char> m_source;
            std::string_view m_name;

            vk::ShaderModule m_shader;
            vk::PipelineShaderStageCreateInfo m_pipelineInfo;
        };
    } // namespace vulkan
} // namespace engine
} // namespace tst
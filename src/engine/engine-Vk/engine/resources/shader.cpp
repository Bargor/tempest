// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "shader.h"

#include "../vulkan_exception.h"

#include <application/data_loader.h>
#include <fmt/printf.h>

namespace tst {
namespace engine {
    namespace vulkan {

        vk::ShaderStageFlagBits get_native_shader_type(shader::shader_type type) {
            switch (type) {
            case tst::engine::vulkan::shader::shader_type::vertex: return vk::ShaderStageFlagBits::eVertex;
            case tst::engine::vulkan::shader::shader_type::fragment: return vk::ShaderStageFlagBits::eFragment;
            case tst::engine::vulkan::shader::shader_type::geometry: return vk::ShaderStageFlagBits::eGeometry;
            case tst::engine::vulkan::shader::shader_type::compute: return vk::ShaderStageFlagBits::eCompute;
            case tst::engine::vulkan::shader::shader_type::tessellation_control:
                return vk::ShaderStageFlagBits::eTessellationControl;
            case tst::engine::vulkan::shader::shader_type::tessellation_evaluation:
                return vk::ShaderStageFlagBits::eTessellationEvaluation;
            default: break;
            }
            return vk::ShaderStageFlagBits::eAll;
        }

        shader::shader(vk::Device device, shader_type type, std::vector<char>&& source, const std::string_view name)
            : m_device(device), m_source(source), m_name(name) {
            vk::ShaderModuleCreateInfo createInfo(
                vk::ShaderModuleCreateFlags(), source.size(), reinterpret_cast<const uint32_t*>(source.data()));

            try {
                m_shader = device.createShaderModule(createInfo);
            } catch (std::runtime_error& ex) {
                throw vulkan_exception(fmt::sprintf("Can't create shader %s\n", ex.what()));
            }

            m_pipelineInfo =
                vk::PipelineShaderStageCreateInfo(vk::PipelineShaderStageCreateFlags(), get_native_shader_type(type), m_shader, "main");
        }

        shader::~shader() {
            if (m_shader.operator VkShaderModule()) {
                m_device.destroyShaderModule(m_shader);
            }
        }

        vk::PipelineShaderStageCreateInfo shader::get_create_info() const {
            return m_pipelineInfo;
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
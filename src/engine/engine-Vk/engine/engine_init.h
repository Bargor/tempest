// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <memory>
#include <optional>
#include <vector>
#include <vulkan/vulkan.hpp>

struct GLFWwindow;

namespace tst {
namespace engine {
    namespace vulkan {

        struct queue_family_indices;
        class shader_compiler;
        class vertex_buffer;

        template<typename T>
        using ptr = std::unique_ptr<T>;

        vk::RenderPass create_render_pass(const vk::Device& device, const vk::Format& format);

        vk::PipelineLayout create_pipeline_layout(const vk::Device& device);
        vk::Pipeline create_graphics_pipeline(const vk::Device& device,
                                              const vk::PipelineLayout& pipelineLayout,
                                              const vk::RenderPass& renderPass,
                                              const vk::Extent2D& extent,
                                              const vulkan::shader_compiler& m_shaderCompiler);

        std::vector<vk::Framebuffer> create_framebuffers(const vk::Device& device,
                                                         const vk::RenderPass& renderPass,
                                                         const std::vector<vk::ImageView>& imageViews,
                                                         const vk::Extent2D& extent);

        vk::CommandPool create_command_pool(const vk::Device& device, const queue_family_indices& indices);

        std::vector<vk::CommandBuffer> create_command_buffers(const vk::Device& device,
                                                              const vk::CommandPool& commandPool,
                                                              const std::vector<vk::Framebuffer>& framebuffers,
                                                              const vk::RenderPass& renderPass,
                                                              const vk::Pipeline& pipeline,
                                                              const vk::Extent2D& extent,
                                                              const vertex_buffer& vertexBuffer);

    } // namespace vulkan
} // namespace engine
} // namespace tst
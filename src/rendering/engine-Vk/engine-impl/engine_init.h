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
        template<typename IndexType>
        class index_buffer;
        class uniform_buffer;
        class device;

        template<typename T>
        using ptr = std::unique_ptr<T>;

        vk::RenderPass create_render_pass(const vk::Device& device, const vk::Format& format);

        vk::PipelineLayout create_pipeline_layout(const vk::Device& device,
                                                  const vk::DescriptorSetLayout& descriptorSetLayout);
        vk::DescriptorSetLayout create_descriptor_set_layout(const vk::Device& device);
        vk::Pipeline create_graphics_pipeline(const vk::Device& device,
                                              const vk::PipelineLayout& pipelineLayout,
                                              const vk::RenderPass& renderPass,
                                              const vk::Extent2D& extent,
                                              const vulkan::shader_compiler& m_shaderCompiler);

        std::vector<vk::Framebuffer> create_framebuffers(const vk::Device& device,
                                                         const vk::RenderPass& renderPass,
                                                         const std::vector<vk::ImageView>& imageViews,
                                                         const vk::Extent2D& extent);

        vk::DescriptorPool create_descriptor_pool(const vk::Device& device, std::size_t size);

        std::vector<uniform_buffer> create_uniform_buffers(const device& device,
                                                           const vk::CommandPool& cmdPool,
                                                           std::size_t size);

        std::vector<vk::DescriptorSet> create_descriptor_sets(const vk::Device& device,
                                                              std::size_t size,
                                                              const vk::DescriptorPool& descriptorPool,
                                                              const vk::DescriptorSetLayout& descriptorSetLayout,
                                                              const std::vector<uniform_buffer>& uniformBuffers);

        std::vector<vk::CommandBuffer> create_command_buffers(const vk::Device& device,
                                                              const vk::CommandPool& commandPool,
                                                              const std::vector<vk::Framebuffer>& framebuffers,
                                                              const vk::RenderPass& renderPass,
                                                              const vk::Pipeline& pipeline,
                                                              const vk::Extent2D& extent,
                                                              const vertex_buffer& vertexBuffer,
                                                              const index_buffer<std::uint16_t>& indexBuffer,
                                                              const vk::PipelineLayout& pipelineLayout,
                                                              const std::vector<vk::DescriptorSet>& descriptorSets);

    } // namespace vulkan
} // namespace engine
} // namespace tst
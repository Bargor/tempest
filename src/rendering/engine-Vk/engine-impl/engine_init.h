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

    } // namespace vulkan
} // namespace engine
} // namespace tst

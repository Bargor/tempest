// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "engine_init.h"

#include "index_buffer.h"
#include "queue_indices.h"
#include "resources/shader_compiler.h"
#include "uniform_buffer.h"
#include "vertex_buffer.h"
#include "vulkan_exception.h"

#include <cstdlib>
#include <fmt/printf.h>
#include <vector>

namespace tst {
namespace engine {
    namespace vulkan {

        vk::RenderPass create_render_pass(const vk::Device& device, const vk::Format& format) {
            vk::AttachmentDescription colorAttachment(vk::AttachmentDescriptionFlags(),
                                                      format,
                                                      vk::SampleCountFlagBits::e1,
                                                      vk::AttachmentLoadOp::eClear,
                                                      vk::AttachmentStoreOp::eStore,
                                                      vk::AttachmentLoadOp::eDontCare,
                                                      vk::AttachmentStoreOp::eDontCare,
                                                      vk::ImageLayout::eUndefined,
                                                      vk::ImageLayout::ePresentSrcKHR);

            vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);

            vk::SubpassDescription subpass(
                vk::SubpassDescriptionFlags(), vk::PipelineBindPoint::eGraphics, 0, nullptr, 1, &colorAttachmentRef);

            vk::SubpassDependency dependency(VK_SUBPASS_EXTERNAL,
                                             0,
                                             vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                             vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                             vk::AccessFlags(),
                                             vk::AccessFlagBits::eColorAttachmentRead |
                                                 vk::AccessFlagBits::eColorAttachmentWrite);

            vk::RenderPassCreateInfo renderPassInfo(
                vk::RenderPassCreateFlags(), 1, &colorAttachment, 1, &subpass, 1, &dependency);

            return device.createRenderPass(renderPassInfo);
        }

        vk::PipelineLayout create_pipeline_layout(const vk::Device& device,
                                                  const vk::DescriptorSetLayout& descriptorSetLayout) {
            vk::PipelineLayoutCreateInfo pipelineLayoutInfo(
                vk::PipelineLayoutCreateFlags(), 1, &descriptorSetLayout, 0, nullptr);
            return device.createPipelineLayout(pipelineLayoutInfo);
        }

        vk::DescriptorSetLayout create_descriptor_set_layout(const vk::Device& device) {
            vk::DescriptorSetLayoutBinding descriptorBinding(
                0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex, nullptr);

            vk::DescriptorSetLayoutCreateInfo setLayoutInfo(vk::DescriptorSetLayoutCreateFlags(), 1, &descriptorBinding);

            return device.createDescriptorSetLayout(setLayoutInfo, nullptr);
        }

        vk::Pipeline create_graphics_pipeline(const vk::Device& device,
                                              const vk::PipelineLayout& pipelineLayout,
                                              const vk::RenderPass& renderPass,
                                              const vk::Extent2D& extent,
                                              const vulkan::shader_compiler& shaderCompiler) {
            vertex_format format;

            vk::PipelineVertexInputStateCreateInfo vertexInfo(
                vk::PipelineVertexInputStateCreateFlags(),
                1,
                &format.get_binding_description(),
                static_cast<std::uint32_t>(format.get_attribute_descriptions().size()),
                format.get_attribute_descriptions().data());
            vk::PipelineInputAssemblyStateCreateInfo assemblyInfo(
                vk::PipelineInputAssemblyStateCreateFlags(), vk::PrimitiveTopology::eTriangleList, false);

            vk::Viewport viewport(
                0.0f, 0.0f, static_cast<float>(extent.width), static_cast<float>(extent.height), 0.0f, 1.0f);
            vk::Rect2D scissor({0, 0}, extent);

            vk::PipelineViewportStateCreateInfo viewportState(
                vk::PipelineViewportStateCreateFlags(), 1, &viewport, 1, &scissor);

            vk::PipelineRasterizationStateCreateInfo rasterizer(vk::PipelineRasterizationStateCreateFlags(),
                                                                false,
                                                                false,
                                                                vk::PolygonMode::eFill,
                                                                vk::CullModeFlagBits::eBack,
                                                                vk::FrontFace::eClockwise,
                                                                false,
                                                                0.0f,
                                                                0.0f,
                                                                0.0f,
                                                                1.0f);

            vk::PipelineMultisampleStateCreateInfo multisampling(
                vk::PipelineMultisampleStateCreateFlags(), vk::SampleCountFlagBits::e1, false, 1.0f, nullptr, false, false);

            vk::PipelineColorBlendAttachmentState colorBlendAttachment(
                false,
                vk::BlendFactor::eOne,
                vk::BlendFactor::eZero,
                vk::BlendOp::eAdd,
                vk::BlendFactor::eOne,
                vk::BlendFactor::eZero,
                vk::BlendOp::eAdd,
                vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB |
                    ::vk::ColorComponentFlagBits::eA);

            vk::PipelineColorBlendStateCreateInfo colorBlending(vk::PipelineColorBlendStateCreateFlags(),
                                                                false,
                                                                vk::LogicOp::eCopy,
                                                                1,
                                                                &colorBlendAttachment,
                                                                {0.0f, 0.0f, 0.0f, 0.0f});

            auto shaders = shaderCompiler.compile_shaders("test");
            std::vector<vk::PipelineShaderStageCreateInfo> shaderInfos;
            std::transform(shaders.cbegin(), shaders.cend(), std::back_inserter(shaderInfos), [](const shader& shader) {
                return shader.get_create_info();
            });

            vk::GraphicsPipelineCreateInfo pipelineInfo(vk::PipelineCreateFlags(),
                                                        2,
                                                        shaderInfos.data(),
                                                        &vertexInfo,
                                                        &assemblyInfo,
                                                        nullptr,
                                                        &viewportState,
                                                        &rasterizer,
                                                        &multisampling,
                                                        nullptr,
                                                        &colorBlending,
                                                        nullptr,
                                                        pipelineLayout,
                                                        renderPass,
                                                        0,
                                                        vk::Pipeline(),
                                                        0);
            return device.createGraphicsPipeline(vk::PipelineCache(), pipelineInfo);
        }

        std::vector<vk::Framebuffer> create_framebuffers(const vk::Device& device,
                                                         const vk::RenderPass& renderPass,
                                                         const std::vector<vk::ImageView>& imageViews,
                                                         const vk::Extent2D& extent) {
            std::vector<vk::Framebuffer> framebuffers(imageViews.size());

            for (std::uint32_t i = 0; i < imageViews.size(); i++) {
                vk::ImageView attachments[] = {imageViews[i]};

                vk::FramebufferCreateInfo createInfo(
                    vk::FramebufferCreateFlags(), renderPass, 1, attachments, extent.width, extent.height, 1);

                framebuffers[i] = device.createFramebuffer(createInfo);
            }

            return framebuffers;
        }

        vk::CommandPool create_command_pool(const vk::Device& device, const queue_family_indices& indices) {
            vk::CommandPoolCreateInfo createInfo(vk::CommandPoolCreateFlags(), indices.graphicsIndex.value());

            return device.createCommandPool(createInfo);
        }

        vk::DescriptorPool create_descriptor_pool(const vk::Device& device, std::size_t size) {
            vk::DescriptorPoolSize poolSize(vk::DescriptorType::eUniformBuffer, static_cast<std::uint32_t>(size));

            vk::DescriptorPoolCreateInfo poolCreateInfo(
                vk::DescriptorPoolCreateFlags(), static_cast<std::uint32_t>(size), 1, &poolSize);

            return device.createDescriptorPool(poolCreateInfo);
        }

        std::vector<uniform_buffer> create_uniform_buffers(const device& device,
                                                           const vk::CommandPool& cmdPool,
                                                           std::size_t size) {
            std::vector<uniform_buffer> buffers;
            while (size--) {
                buffers.emplace_back(uniform_buffer(device, cmdPool));
            }
            return buffers;
        }

        std::vector<vk::DescriptorSet> create_descriptor_sets(const vk::Device& device,
                                                              std::size_t size,
                                                              const vk::DescriptorPool& descriptorPool,
                                                              const vk::DescriptorSetLayout& descriptorSetLayout,
                                                              const std::vector<uniform_buffer>& uniformBuffers) {
            std::vector<vk::DescriptorSetLayout> layouts(size, descriptorSetLayout);
            vk::DescriptorSetAllocateInfo allocInfo(descriptorPool, static_cast<std::uint32_t>(size), layouts.data());

            auto descriptorSets = device.allocateDescriptorSets(allocInfo);

            for (std::size_t i = 0; i < size; ++i) {
                vk::DescriptorBufferInfo bufferInfo(uniformBuffers[i].get_handle(), 0, sizeof(uniform_buffer_object));

                vk::WriteDescriptorSet descriptorWrite(
                    descriptorSets[i], 0, 0, 1, vk::DescriptorType::eUniformBuffer, nullptr, &bufferInfo, nullptr);

                device.updateDescriptorSets({descriptorWrite}, {});
            }

            return descriptorSets;
        }

        std::vector<vk::CommandBuffer> create_command_buffers(const vk::Device& device,
                                                              const vk::CommandPool& commandPool,
                                                              const std::vector<vk::Framebuffer>& framebuffers,
                                                              const vk::RenderPass& renderPass,
                                                              const vk::Pipeline& pipeline,
                                                              const vk::Extent2D& extent,
                                                              const vertex_buffer& vertexBuffer,
                                                              const index_buffer& indexBuffer,
                                                              const vk::PipelineLayout& pipelineLayout,
                                                              const std::vector<vk::DescriptorSet>& descriptorSets) {
            vk::CommandBufferAllocateInfo bufferAllocateInfo(
                commandPool, vk::CommandBufferLevel::ePrimary, static_cast<std::uint32_t>(framebuffers.size()));

            auto commandBuffers = device.allocateCommandBuffers(bufferAllocateInfo);

            for (size_t i = 0; i < commandBuffers.size(); i++) {
                vk::CommandBufferBeginInfo commandBufferInfo(vk::CommandBufferUsageFlagBits::eSimultaneousUse, nullptr);
                commandBuffers[i].begin(commandBufferInfo);

                vk::ClearValue clearColor = vk::ClearValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});

                vk::RenderPassBeginInfo renderPassInfo(
                    renderPass, framebuffers[i], vk::Rect2D({0, 0}, extent), 1, &clearColor);

                commandBuffers[i].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
                commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);

                std::vector<vk::Buffer> vertexBuffers = {vertexBuffer.get_handle()};
                std::vector<vk::DeviceSize> offsets = {0};
                commandBuffers[i].bindVertexBuffers(0, vertexBuffers, offsets);
                commandBuffers[i].bindIndexBuffer(indexBuffer.get_handle(), 0, vk::IndexType::eUint16);
                commandBuffers[i].bindDescriptorSets(
                    vk::PipelineBindPoint::eGraphics, pipelineLayout, 0, descriptorSets[i], {});
                commandBuffers[i].drawIndexed(indexBuffer.get_index_count(), 1, 0, 0, 0);
                commandBuffers[i].endRenderPass();
                commandBuffers[i].end();
            }

            return commandBuffers;
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst

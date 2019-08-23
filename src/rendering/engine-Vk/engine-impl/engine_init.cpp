// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "engine_init.h"

#include "device.h"
#include "queue_indices.h"
#include "resources/index_buffer.h"
#include "resources/uniform_buffer.h"
#include "resources/vertex_buffer.h"
#include "shader_compiler.h"
#include "vulkan_exception.h"

#include <cstdlib>
#include <fmt/printf.h>
#include <vector>

namespace tst {
namespace engine {
    namespace vulkan {

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
                return shader.get_pipeline_info();
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
                buffers.emplace_back(device.create_uniform_buffer(cmdPool));
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

    } // namespace vulkan
} // namespace engine
} // namespace tst

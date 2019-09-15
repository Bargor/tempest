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

        vk::DescriptorSetLayout create_descriptor_set_layout(const vk::Device& device) {
            vk::DescriptorSetLayoutBinding descriptorBinding(
                0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex, nullptr);

            vk::DescriptorSetLayoutCreateInfo setLayoutInfo(vk::DescriptorSetLayoutCreateFlags(), 1, &descriptorBinding);

            return device.createDescriptorSetLayout(setLayoutInfo, nullptr);
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

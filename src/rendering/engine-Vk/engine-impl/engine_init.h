// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <memory>
#include <optional>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {

        class uniform_buffer;
        class device;

        vk::DescriptorSetLayout create_descriptor_set_layout(const vk::Device& device);

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

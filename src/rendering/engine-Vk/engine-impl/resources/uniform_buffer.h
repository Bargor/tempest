// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "buffer.h"
#include "settings.h"

#include <glm.h>
#include <vector>

namespace tst {
namespace engine {
    namespace vulkan {

        using descriptor_set = std::array<vk::DescriptorSet, settings::m_inFlightFrames>;

        class uniform_buffer : public buffer {
            using super = buffer;

        public:
            uniform_buffer(vk::Device logicalDevice,
                           vk::Queue m_queueHandle,
                           vk::CommandPool cmdPool,
                           const descriptor_set& descriptorSets,
                           std::uint32_t binding,
                           vk::PhysicalDeviceMemoryProperties memoryProperties,
                           const std::uint32_t& resourceIndex,
                           std::size_t storageSize);
            uniform_buffer(uniform_buffer&& other) noexcept;
            uniform_buffer(const uniform_buffer& other) = delete;

            ~uniform_buffer() = default;

            void update_buffer(const void* data, const std::size_t dataSize);
            vk::DescriptorSet get_descriptor_set() const noexcept;

        private:
            const std::uint32_t& m_resourceIndex;
            const std::array<vk::DescriptorSet, settings::m_inFlightFrames>& m_descriptorSets;
            std::uint32_t m_binding;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst

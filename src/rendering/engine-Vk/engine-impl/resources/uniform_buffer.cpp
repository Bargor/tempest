// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "uniform_buffer.h"

#include "../vulkan_exception.h"
#include "settings.h"

namespace tst {
namespace engine {
    namespace vulkan {

        uniform_buffer::uniform_buffer(vk::Device logicalDevice,
                                       vk::Queue queueHandle,
                                       vk::CommandPool cmdPool,
                                       vk::DescriptorPool descPool,
                                       vk::DescriptorSetLayout descLayout,
                                       base::uniform_bind_point bindPoint,
                                       std::uint32_t binding,
                                       vk::PhysicalDeviceMemoryProperties memoryProperties,
                                       const std::uint32_t& resourceIndex,
                                       std::size_t storageSize)
            : buffer(logicalDevice,
                     queueHandle,
                     cmdPool,
                     storageSize * settings::m_inFlightFrames,
                     vk::BufferUsageFlagBits::eUniformBuffer,
                     memoryProperties,
                     vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible)
            , m_setNumber(bindPoint)
            , m_binding(binding)
            , m_resourceIndex(resourceIndex) {
            std::vector<vk::DescriptorSetLayout> layouts(settings::m_inFlightFrames, descLayout);
            const vk::DescriptorSetAllocateInfo allocInfo(descPool, settings::m_inFlightFrames, layouts.data());

            const auto descriptorSets = logicalDevice.allocateDescriptorSets(allocInfo);

            for (std::size_t i = 0; i < settings::m_inFlightFrames; ++i) {
                const vk::DescriptorBufferInfo bufferInfo(m_buffer, i * storageSize, storageSize);

                const vk::WriteDescriptorSet descriptorWrite(
                    descriptorSets[i], 0, 0, 1, vk::DescriptorType::eUniformBuffer, nullptr, &bufferInfo, nullptr);

                logicalDevice.updateDescriptorSets({descriptorWrite}, {});

                m_descriptorSets[i] = descriptorSets[i];
            }
        }

        uniform_buffer::uniform_buffer(uniform_buffer&& other) noexcept
            : buffer(std::move(other))
            , m_setNumber(other.m_setNumber)
            , m_binding(other.m_binding)
            , m_resourceIndex(other.m_resourceIndex)
            , m_descriptorSets(std::move(other.m_descriptorSets)) {
        }

        void uniform_buffer::update_buffer(const void* data, const std::size_t dataSize) {
            const auto dataPtr = m_logicalDevice.mapMemory(m_bufferMemory, m_resourceIndex * dataSize, dataSize);
            std::memcpy(dataPtr, data, dataSize);
            m_logicalDevice.unmapMemory(m_bufferMemory);
        }

        vk::DescriptorSet uniform_buffer::get_descriptor_set() const noexcept {
            return m_descriptorSets[m_resourceIndex];
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst

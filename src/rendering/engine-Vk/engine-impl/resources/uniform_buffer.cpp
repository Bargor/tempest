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
                                       vk::PhysicalDeviceMemoryProperties memoryProperties,
                                       const std::uint32_t& resourceIndex)
            : buffer(logicalDevice,
                     queueHandle,
                     cmdPool,
                     sizeof(uniform_buffer_object) * settings::m_inFlightFrames,
                     vk::BufferUsageFlagBits::eUniformBuffer,
                     memoryProperties,
                     vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible)
            , m_resourceIndex(resourceIndex) {
            std::vector<vk::DescriptorSetLayout> layouts(settings::m_inFlightFrames, descLayout);
            vk::DescriptorSetAllocateInfo allocInfo(descPool, settings::m_inFlightFrames, layouts.data());

            auto descriptorSets = logicalDevice.allocateDescriptorSets(allocInfo);

            for (std::size_t i = 0; i < settings::m_inFlightFrames; ++i) {
                vk::DescriptorBufferInfo bufferInfo(
                    m_buffer, i * sizeof(uniform_buffer_object), sizeof(uniform_buffer_object));

                vk::WriteDescriptorSet descriptorWrite(
                    descriptorSets[i], 0, 0, 1, vk::DescriptorType::eUniformBuffer, nullptr, &bufferInfo, nullptr);

                logicalDevice.updateDescriptorSets({descriptorWrite}, {});

                m_descriptorSets[i] = descriptorSets[i];
            }
        }

        uniform_buffer::~uniform_buffer() {
        }

        uniform_buffer::uniform_buffer(uniform_buffer&& other) noexcept
            : buffer(std::move(other))
            , m_data(std::move(other.m_data))
            , m_resourceIndex(other.m_resourceIndex)
            , m_descriptorSets(std::move(other.m_descriptorSets)) {
        }

        void uniform_buffer::update_buffer(const uniform_buffer_object& ubo) {
            m_data = ubo;
            auto dataPtr = m_logicalDevice.mapMemory(
                m_bufferMemory, m_resourceIndex * sizeof(uniform_buffer_object), sizeof(uniform_buffer_object));
            std::memcpy(dataPtr, &ubo, sizeof(uniform_buffer_object));
            m_logicalDevice.unmapMemory(m_bufferMemory);
        }

        vk::DescriptorSet uniform_buffer::get_descriptor_set() const noexcept {
            return m_descriptorSets[m_resourceIndex];
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst

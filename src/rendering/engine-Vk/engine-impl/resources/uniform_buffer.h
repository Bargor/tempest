// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "buffer.h"
#include "settings.h"

#include <engine-base/uniform_buffer.h>
#include <glm.h>
#include <vector>

namespace tst {
namespace engine {
    namespace vulkan {

        class uniform_buffer : public buffer {
            using super = buffer;

        public:
            uniform_buffer(vk::Device logicalDevice,
                           vk::Queue m_queueHandle,
                           vk::CommandPool cmdPool,
                           vk::DescriptorSetLayout descLayout,
                           base::resource_bind_point bindPoint,
                           std::uint32_t binding,
                           vk::PhysicalDeviceMemoryProperties memoryProperties,
                           const std::uint32_t& resourceIndex,
                           std::size_t storageSize);
            uniform_buffer(uniform_buffer&& other) noexcept;
            uniform_buffer(const uniform_buffer& other) = delete;

            ~uniform_buffer() = default;

            void update_buffer(const void* data, const std::size_t dataSize);
            vk::DescriptorSet get_descriptor_set() const noexcept;
            std::uint32_t get_binding() const noexcept;
            base::resource_bind_point get_bind_point() const noexcept;

        private:
            base::resource_bind_point m_setNumber;
            std::uint32_t m_binding;
            const std::uint32_t& m_resourceIndex;
            std::array<vk::DescriptorSet, settings::m_inFlightFrames> m_descriptorSets;
        };

        TST_INLINE std::uint32_t uniform_buffer::get_binding() const noexcept {
            return m_binding;
        }

        TST_INLINE base::resource_bind_point uniform_buffer::get_bind_point() const noexcept {
            return m_setNumber;
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst

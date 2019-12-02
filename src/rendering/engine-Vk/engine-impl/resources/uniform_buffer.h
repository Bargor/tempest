// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "buffer.h"
#include "settings.h"

#include <glm/glm.hpp>
#include <vector>

namespace tst {
namespace engine {
    namespace vulkan {

        struct uniform_buffer_object {
            glm::mat4 model;
            glm::mat4 view;
            glm::mat4 proj;
            glm::mat4 asd;
        };

        class uniform_buffer : public buffer {
        public:
            using super = buffer;

            uniform_buffer(vk::Device logicalDevice,
                           vk::Queue m_queueHandle,
                           vk::CommandPool cmdPool,
                           vk::DescriptorPool descPool,
                           vk::DescriptorSetLayout descLayout,
                           const vk::PhysicalDeviceMemoryProperties& memoryProperties,
                           const std::uint32_t& resourceIndex);
            uniform_buffer(uniform_buffer&& other) noexcept;
            uniform_buffer(const uniform_buffer& other) = delete;

            ~uniform_buffer();

            void update_buffer(const uniform_buffer_object& ubo);
            vk::DescriptorSet get_descriptor_set() const noexcept;

        private:
            uniform_buffer_object m_data;
            const std::uint32_t& m_resourceIndex;
            std::array<vk::DescriptorSet, settings::m_inFlightFrames> m_descriptorSets;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst

// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "buffer.h"

#include <glm/glm.hpp>
#include <vector>

namespace tst {
namespace engine {
    namespace vulkan {

        struct uniform_buffer_object {
            glm::mat4 model;
            glm::mat4 view;
            glm::mat4 proj;
        };

        class uniform_buffer : public buffer {
        public:
            using super = buffer;

            uniform_buffer(vk::Device logicalDevice,
                           vk::Queue m_queueHandle,
                           vk::CommandPool cmdPool,
                           const vk::PhysicalDeviceMemoryProperties& memoryProperties,
                           const std::uint32_t& resourceIndex);
            uniform_buffer(uniform_buffer&& other) noexcept;
            uniform_buffer(const uniform_buffer& other) = delete;

            ~uniform_buffer();

            void update_buffer(const uniform_buffer_object& ubo);

        private:
            uniform_buffer_object m_data;
            const std::uint32_t& m_resourceIndex;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst

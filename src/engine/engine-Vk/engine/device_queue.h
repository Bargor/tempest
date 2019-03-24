// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <cstdint>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {

    namespace vulkan {

        struct queue_family_indices;

        class device_queues {
        public:
            device_queues(const vk::Device& logicalDevice, const queue_family_indices& queueFamilyIndices);

            ~device_queues();

        public:
            vk::Queue m_graphicsQueueHandle;
            vk::Queue m_computeQueueHandle;
            vk::Queue m_presentationQueueHandle;
            vk::Queue m_transferQueueHandle;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
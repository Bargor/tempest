// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "queue_indices.h"

#include <vector>
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

namespace tst {
namespace engine {
    namespace vulkan {

        class device {
            friend class rendering_engine;
            friend class swap_chain;
            friend class buffer;
            friend class vertex_buffer;

        public:
            device(GLFWwindow* window, const std::vector<const char*>& requiredExtensions);
            ~device();

        private:
            vk::SurfaceKHR m_windowSurface;
            vk::PhysicalDevice m_physicalDevice;
            queue_family_indices m_queueIndices;
            vk::Device m_logicalDevice;
            vk::Queue m_graphicsQueueHandle;
            vk::Queue m_computeQueueHandle;
            vk::Queue m_presentationQueueHandle;
            vk::Queue m_transferQueueHandle;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst

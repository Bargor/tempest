// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace tst {
namespace engine {

    namespace vulkan {

        class physical_device;

        class swap_chain {
        public:

            struct support_details {
                VkSurfaceCapabilitiesKHR capabilities;
                std::vector<VkSurfaceFormatKHR> formats;
                std::vector<VkPresentModeKHR> presentModes;
            };

            swap_chain(const physical_device& physicalDevice,
                       const VkDevice& logicalDevice,
                       const VkSurfaceKHR& m_windowSurface,
                       std::uint32_t width,
                       std::uint32_t height);

            ~swap_chain();

        private:
            support_details check_support(const physical_device& physicalDevice, const VkSurfaceKHR& m_windowSurface);

        private:
            support_details m_supportDetails;
            VkSurfaceFormatKHR m_surfaceFormat;
            VkPresentModeKHR m_presentationMode;
            VkExtent2D m_extent;
            const VkDevice& m_logicalDevice;

            VkSwapchainKHR m_swapChain;
        };

    }
}
}
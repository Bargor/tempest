// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace tst {
namespace engine {

    namespace vulkan {

        class swap_chain {
        public:

            struct SwapChainSupportDetails {
                VkSurfaceCapabilitiesKHR capabilities;
                std::vector<VkSurfaceFormatKHR> formats;
                std::vector<VkPresentModeKHR> presentModes;
            };

        private:

            VkSwapchainKHR m_swapChain;
        };

    }
}
}
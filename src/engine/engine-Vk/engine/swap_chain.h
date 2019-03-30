// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>

namespace tst {
namespace engine {

    namespace vulkan {

        class physical_device;
        struct queue_family_indices;

        class swap_chain {
        public:

            struct support_details {
                vk::SurfaceCapabilitiesKHR capabilities;
                std::vector<vk::SurfaceFormatKHR> formats;
                std::vector<vk::PresentModeKHR> presentModes;
            };

            swap_chain(const vk::PhysicalDevice& physicalDevice,
                       const vk::Device& logicalDevice,
                       const vk::SurfaceKHR& m_windowSurface,
                       const queue_family_indices& indices,
                       std::uint32_t width,
                       std::uint32_t height);

            ~swap_chain();
        private:
            support_details m_supportDetails;
            vk::SurfaceFormatKHR m_surfaceFormat;
            vk::PresentModeKHR m_presentationMode;
            vk::Extent2D m_extent;
            const vk::Device& m_logicalDevice;
            std::uint32_t m_imagesCount;

            vk::SwapchainKHR m_swapChain;
            std::vector<vk::Image> m_images;
            std::vector<vk::ImageView> m_imageViews;
        };

    }
}
}
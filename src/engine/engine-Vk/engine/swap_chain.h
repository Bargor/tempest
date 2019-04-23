// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>

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
                       const vk::SurfaceKHR& windowSurface,
                       const queue_family_indices& indices,
                       std::uint32_t width,
                       std::uint32_t height);

            ~swap_chain();

            const vk::Extent2D& get_extent() const;
            const vk::Format& get_format() const;
            const std::vector<vk::ImageView>& get_image_views() const;
            const vk::SwapchainKHR& get_native_swapchain() const;

        private:
            const vk::Device& m_logicalDevice;
            const vk::SurfaceKHR& m_windowSurface;
            const queue_family_indices& m_indices;
            support_details m_supportDetails;
            vk::SurfaceFormatKHR m_surfaceFormat;
            vk::PresentModeKHR m_presentationMode;
            vk::Extent2D m_extent;
            std::uint32_t m_imagesCount;

            vk::SwapchainKHR m_swapChain;
            std::vector<vk::Image> m_images;
            std::vector<vk::ImageView> m_imageViews;
        };

        inline const vk::Extent2D& swap_chain::get_extent() const {
            return m_extent;
        }

        inline const vk::Format& swap_chain::get_format() const {
            return m_surfaceFormat.format;
        }

        inline const std::vector<vk::ImageView>& swap_chain::get_image_views() const {
            return m_imageViews;
        }

        inline const vk::SwapchainKHR& swap_chain::get_native_swapchain() const {
            return m_swapChain;
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>

namespace tst {

namespace engine {

    namespace vulkan {

        class device;
        struct queue_family_indices;

        class swap_chain {
        public:
            enum class result { success, resize, fail };

            struct support_details {
                vk::SurfaceCapabilitiesKHR capabilities;
                std::vector<vk::SurfaceFormatKHR> formats;
                std::vector<vk::PresentModeKHR> presentModes;
            };

            swap_chain(const vk::Device& device,
                       const vk::PhysicalDevice& physicalDevice,
                       const vk::SurfaceKHR& windowSurface,
                       const queue_family_indices& indices,
                       std::uint32_t width,
                       std::uint32_t height);

            ~swap_chain();

            const vk::Extent2D& get_extent() const;
            const vk::Format& get_format() const;
            const std::vector<vk::ImageView>& get_image_views() const;
            const vk::SwapchainKHR& get_native_swapchain() const;
            result acquire_next_image(const vk::Device& device, const vk::Semaphore& imageAvailable);
            result present_image(const vk::Queue& presentationQueueHandle, const vk::Semaphore& renderFinished);
            std::uint32_t get_image_index() const;

        private:
            const vk::Device& m_logicalDevice;
            const vk::SurfaceKHR& m_windowSurface;
            support_details m_supportDetails;
            vk::SurfaceFormatKHR m_surfaceFormat;
            vk::PresentModeKHR m_presentationMode;
            vk::Extent2D m_extent;
            std::uint32_t m_imagesCount;

            vk::SwapchainKHR m_swapChain;
            std::vector<vk::Image> m_images;
            std::vector<vk::ImageView> m_imageViews;
            std::uint32_t m_currentImage;
        };

        TST_INLINE const vk::Extent2D& swap_chain::get_extent() const {
            return m_extent;
        }

        TST_INLINE const vk::Format& swap_chain::get_format() const {
            return m_surfaceFormat.format;
        }

        TST_INLINE const std::vector<vk::ImageView>& swap_chain::get_image_views() const {
            return m_imageViews;
        }

        TST_INLINE const vk::SwapchainKHR& swap_chain::get_native_swapchain() const {
            return m_swapChain;
        }

        TST_INLINE std::uint32_t swap_chain::get_image_index() const {
            return m_currentImage;
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst

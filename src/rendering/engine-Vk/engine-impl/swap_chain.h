// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "physical_device.h"
#include "resources/settings.h"

#include <vector>
#include <vulkan/vulkan.hpp>

namespace tst {

namespace engine {

    namespace vulkan {

        class device;
        class physical_device;
        struct queue_family_indices;

        class swap_chain {
        public:
            enum class result { success, resize, fail };

            swap_chain(const physical_device& physicalDevice,
                       vk::Device device,
                       vk::SurfaceKHR windowSurface,
                       const surface_support_info& info,
                       std::uint32_t graphicsQueueIndex,
                       std::uint32_t presentationQueueIndex,
                       const vk::Extent2D extent,
                       settings::buffering buffers);

            ~swap_chain();

            const vk::Extent2D& get_extent() const noexcept;
            const vk::Format& get_format() const noexcept;
            const std::vector<vk::ImageView>& get_image_views() const noexcept;
            const vk::SwapchainKHR& get_native_swapchain() const noexcept;
            result acquire_next_image(vk::Device device, const vk::Semaphore imageAvailable);
            result present_image(vk::Queue presentationQueueHandle, const vk::Semaphore renderFinished);
            std::uint32_t get_image_index() const noexcept;
            settings::buffering get_buffering() const noexcept;

        private:
            const physical_device& m_physicalDevice;
            vk::Device m_logicalDevice;
            vk::SurfaceKHR m_windowSurface;
            surface_support_info m_supportInfo;
            vk::SurfaceFormatKHR m_surfaceFormat;
            vk::PresentModeKHR m_presentationMode;
            vk::Extent2D m_extent;
            settings::buffering m_buffering;

            vk::SwapchainKHR m_swapChain;
            std::vector<vk::Image> m_images;
            std::vector<vk::ImageView> m_imageViews;
            vk::Image m_depthImage;
            vk::DeviceMemory m_depthImageMemory;
            vk::ImageView m_depthImageView;
            std::uint32_t m_currentImage;
        };

        TST_INLINE const vk::Extent2D& swap_chain::get_extent() const noexcept {
            return m_extent;
        }

        TST_INLINE const vk::Format& swap_chain::get_format() const noexcept {
            return m_surfaceFormat.format;
        }

        TST_INLINE const std::vector<vk::ImageView>& swap_chain::get_image_views() const noexcept {
            return m_imageViews;
        }

        TST_INLINE const vk::SwapchainKHR& swap_chain::get_native_swapchain() const noexcept {
            return m_swapChain;
        }

        TST_INLINE std::uint32_t swap_chain::get_image_index() const noexcept {
            return m_currentImage;
        }

        TST_INLINE settings::buffering swap_chain::get_buffering() const noexcept {
            return m_buffering;
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst

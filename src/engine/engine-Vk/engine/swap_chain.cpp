// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "swap_chain.h"

#include "physical_device.h"
#include "vulkan_exception.h"

#include <algorithm>

namespace tst {
namespace engine {

    namespace vulkan {

        namespace {

            VkSurfaceFormatKHR choose_surface_format(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
                if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
                    return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
                }

                for (const auto& availableFormat : availableFormats) {
                    if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
                        availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                        return availableFormat;
                    }
                }

                return availableFormats[0];
            }

            VkPresentModeKHR choose_presentation_mode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
                VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

                for (const auto& availablePresentMode : availablePresentModes) {
                    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                        return availablePresentMode;
                    } else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
                        bestMode = availablePresentMode;
                    }
                }

                return bestMode;
            }

            VkExtent2D choose_extent(const VkSurfaceCapabilitiesKHR& capabilities,
                                     std::uint32_t width,
                                     std::uint32_t height) {
                if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
                    return capabilities.currentExtent;
                } else {
                    VkExtent2D actualExtent = {width, height};

                    actualExtent.width = std::max(capabilities.minImageExtent.width,
                                                  std::min(capabilities.maxImageExtent.width, actualExtent.width));
                    actualExtent.height = std::max(capabilities.minImageExtent.height,
                                                   std::min(capabilities.maxImageExtent.height, actualExtent.height));

                    return actualExtent;
                }
            }

            VkSwapchainKHR create_swap_chain(const VkSurfaceKHR& m_windowSurface,
                                             const VkDevice& logicalDevice,
                                             const physical_device::queue_family_indices& queueIndices,
                                             const swap_chain::support_details& supportDetails,
                                             const VkSurfaceFormatKHR surfaceFormat,
                                             const VkPresentModeKHR& presentationMode,
                                             const VkExtent2D& extent,
                                             std::uint32_t& imageCount) {
                imageCount = supportDetails.capabilities.minImageCount + 1;
                if (supportDetails.capabilities.maxImageCount > 0 &&
                    imageCount > supportDetails.capabilities.maxImageCount) {
                    imageCount = supportDetails.capabilities.maxImageCount;
                }

                VkSwapchainCreateInfoKHR createInfo = {};
                createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
                createInfo.surface = m_windowSurface;
                createInfo.minImageCount = imageCount;
                createInfo.imageFormat = surfaceFormat.format;
                createInfo.imageColorSpace = surfaceFormat.colorSpace;
                createInfo.imageExtent = extent;
                createInfo.imageArrayLayers = 1;
                createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

                uint32_t queueFamilyIndices[] = {queueIndices.graphicsIndex.value(),
                                                 queueIndices.presentationIndex.value()};

                if (queueIndices.graphicsIndex != queueIndices.presentationIndex) {
                    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                    createInfo.queueFamilyIndexCount = 2;
                    createInfo.pQueueFamilyIndices = queueFamilyIndices;
                } else {
                    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
                    createInfo.queueFamilyIndexCount = 0;
                    createInfo.pQueueFamilyIndices = nullptr;
                }
                createInfo.preTransform = supportDetails.capabilities.currentTransform;
                createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
                createInfo.presentMode = presentationMode;
                createInfo.clipped = VK_TRUE;
                createInfo.oldSwapchain = VK_NULL_HANDLE;

                VkSwapchainKHR swapChain;
                if (vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
                    throw vulkan_exception("Failed to create swap chain!");
                }

                return swapChain;
            }

        } // namespace

        swap_chain::swap_chain(const physical_device& physicalDevice,
                               const VkDevice& logicalDevice,
                               const VkSurfaceKHR& m_windowSurface,
                               std::uint32_t width,
                               std::uint32_t height)
            : m_supportDetails(check_support(physicalDevice, m_windowSurface))
            , m_surfaceFormat(choose_surface_format(m_supportDetails.formats))
            , m_presentationMode(choose_presentation_mode(m_supportDetails.presentModes))
            , m_extent(choose_extent(m_supportDetails.capabilities, width, height))
            , m_logicalDevice(logicalDevice)
            , m_swapChain(create_swap_chain(m_windowSurface,
                                            logicalDevice,
                                            physicalDevice.get_queue_family_indices(),
                                            m_supportDetails,
                                            m_surfaceFormat,
                                            m_presentationMode,
                                            m_extent,
                                            m_imagesCount)) {
            vkGetSwapchainImagesKHR(logicalDevice, m_swapChain, &m_imagesCount, nullptr);
            m_images.resize(m_imagesCount);
            vkGetSwapchainImagesKHR(logicalDevice, m_swapChain, &m_imagesCount, m_images.data());

            m_imageViews.resize(m_imagesCount);

            for (std::uint32_t idx = 0; idx < m_imageViews.size(); idx++) {
                VkImageViewCreateInfo createInfo = {};
                createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                createInfo.image = m_images[idx];
                createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
                createInfo.format = m_surfaceFormat.format;
                createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
                createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
                createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
                createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
                createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                createInfo.subresourceRange.baseMipLevel = 0;
                createInfo.subresourceRange.levelCount = 1;
                createInfo.subresourceRange.baseArrayLayer = 0;
                createInfo.subresourceRange.layerCount = 1;

                if (vkCreateImageView(logicalDevice, &createInfo, nullptr, &m_imageViews[idx]) != VK_SUCCESS) {
                    throw vulkan_exception("Failed to create image views!");
                }
            }
        }

        swap_chain::~swap_chain() {
            std::for_each(m_imageViews.begin(), m_imageViews.end(), [this](VkImageView& view) {
                vkDestroyImageView(m_logicalDevice, view, nullptr);
            });
            vkDestroySwapchainKHR(m_logicalDevice, m_swapChain, nullptr);
        }

        swap_chain::support_details swap_chain::check_support(const physical_device& physicalDevice,
                                                              const VkSurfaceKHR& m_windowSurface) {
            swap_chain::support_details details;

            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
                physicalDevice.m_deviceHandle, m_windowSurface, &details.capabilities);

            uint32_t formatCount;
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice.m_deviceHandle, m_windowSurface, &formatCount, nullptr);

            if (formatCount != 0) {
                details.formats.resize(formatCount);
                vkGetPhysicalDeviceSurfaceFormatsKHR(
                    physicalDevice.m_deviceHandle, m_windowSurface, &formatCount, details.formats.data());
            }

            uint32_t presentModeCount;
            vkGetPhysicalDeviceSurfacePresentModesKHR(
                physicalDevice.m_deviceHandle, m_windowSurface, &presentModeCount, nullptr);

            if (presentModeCount != 0) {
                details.presentModes.resize(presentModeCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(
                    physicalDevice.m_deviceHandle, m_windowSurface, &presentModeCount, details.presentModes.data());
            }

            return details;
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst

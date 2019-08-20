// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "swap_chain.h"

#include "device.h"
#include "queue_indices.h"
#include "vulkan_exception.h"

#include <algorithm>
#include <application/event_processor.h>

namespace tst {
namespace engine {

    namespace vulkan {

        namespace {

            vk::SurfaceFormatKHR choose_surface_format(const std::vector<vk::SurfaceFormatKHR>& availableFormats) {
                if (availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined) {
                    return {vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear};
                }

                for (const auto& availableFormat : availableFormats) {
                    if (availableFormat.format == vk::Format::eB8G8R8A8Unorm &&
                        availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                        return availableFormat;
                    }
                }

                return availableFormats[0];
            }

            vk::PresentModeKHR choose_presentation_mode(const std::vector<vk::PresentModeKHR>& availablePresentModes) {
                vk::PresentModeKHR bestMode = vk::PresentModeKHR::eFifo;

                for (const auto& availablePresentMode : availablePresentModes) {
                    if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
                        return availablePresentMode;
                    } else if (availablePresentMode == vk::PresentModeKHR::eImmediate) {
                        bestMode = availablePresentMode;
                    }
                }

                return bestMode;
            }

            vk::Extent2D choose_extent(const vk::SurfaceCapabilitiesKHR& capabilities,
                                       std::uint32_t width,
                                       std::uint32_t height) {
                if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
                    return capabilities.currentExtent;
                } else {
                    vk::Extent2D actualExtent = {width, height};

                    actualExtent.width = std::clamp(
                        actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
                    actualExtent.height = std::clamp(
                        actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

                    return actualExtent;
                }
            }

            vk::SwapchainKHR create_swap_chain(const vk::SurfaceKHR& m_windowSurface,
                                               const vk::Device& logicalDevice,
                                               std::uint32_t graphicsQueueIndex,
                                               std::uint32_t presentationQueueIndex,
                                               const swap_chain::support_details& supportDetails,
                                               const vk::SurfaceFormatKHR surfaceFormat,
                                               const vk::PresentModeKHR& presentationMode,
                                               const vk::Extent2D& extent,
                                               std::uint32_t& imageCount) {
                imageCount = supportDetails.capabilities.minImageCount + 1;
                if (supportDetails.capabilities.maxImageCount > 0 &&
                    imageCount > supportDetails.capabilities.maxImageCount) {
                    imageCount = supportDetails.capabilities.maxImageCount;
                }

                uint32_t queueFamilyIndices[] = {graphicsQueueIndex, presentationQueueIndex};

                vk::SharingMode sharingMode;
                std::uint32_t queueFamilyIndexCount;

                if (graphicsQueueIndex != presentationQueueIndex) {
                    sharingMode = vk::SharingMode::eConcurrent;
                    queueFamilyIndexCount = 2;
                } else {
                    sharingMode = vk::SharingMode::eExclusive;
                    queueFamilyIndexCount = 0;
                }

                vk::SwapchainCreateInfoKHR createInfo(
                    vk::SwapchainCreateFlagsKHR(),
                    m_windowSurface,
                    imageCount,
                    surfaceFormat.format,
                    surfaceFormat.colorSpace,
                    extent,
                    1,
                    vk::ImageUsageFlagBits::eColorAttachment,
                    sharingMode,
                    queueFamilyIndexCount,
                    graphicsQueueIndex != presentationQueueIndex ? queueFamilyIndices : nullptr,
                    supportDetails.capabilities.currentTransform,
                    vk::CompositeAlphaFlagBitsKHR::eOpaque,
                    presentationMode,
                    true,
                    nullptr);

                try {
                    return logicalDevice.createSwapchainKHR(createInfo);
                } catch (std::runtime_error&) {
                    throw vulkan_exception("Failed to create swap chain!");
                }
            }

            std::vector<vk::ImageView> create_image_views(const vk::Device& logicalDevice,
                                                          const vk::SurfaceFormatKHR& surfaceFormat,
                                                          const std::vector<vk::Image>& m_images) {
                std::vector<vk::ImageView> imageViews(m_images.size());

                for (std::uint32_t idx = 0; idx < imageViews.size(); idx++) {
                    vk::ImageAspectFlags flags = vk::ImageAspectFlagBits::eColor;

                    vk::ImageViewCreateInfo createInfo(vk::ImageViewCreateFlags(),
                                                       m_images[idx],
                                                       vk::ImageViewType::e2D,
                                                       surfaceFormat.format,
                                                       vk::ComponentMapping(),
                                                       vk::ImageSubresourceRange(flags, 0, 1, 0, 1));

                    try {
                        imageViews[idx] = logicalDevice.createImageView(createInfo);
                    } catch (std::runtime_error&) {
                        throw vulkan_exception("Failed to create image views!");
                    }
                }
                return imageViews;
            }

            swap_chain::support_details check_support(const vk::PhysicalDevice& physicalDevice,
                                                      const vk::SurfaceKHR& m_windowSurface) {
                swap_chain::support_details details;

                details.capabilities = physicalDevice.getSurfaceCapabilitiesKHR(m_windowSurface);
                details.formats = physicalDevice.getSurfaceFormatsKHR(m_windowSurface);
                details.presentModes = physicalDevice.getSurfacePresentModesKHR(m_windowSurface);

                return details;
            }

        } // namespace

        swap_chain::swap_chain(const vk::Device& device,
                               const vk::PhysicalDevice& physicalDevice,
                               const vk::SurfaceKHR& windowSurface,
                               std::uint32_t graphicsQueueIndex,
                               std::uint32_t presentationQueueIndex,
                               std::uint32_t width,
                               std::uint32_t height)
            : m_logicalDevice(device)
            , m_windowSurface(windowSurface)
            , m_supportDetails(check_support(physicalDevice, m_windowSurface))
            , m_surfaceFormat(choose_surface_format(m_supportDetails.formats))
            , m_presentationMode(choose_presentation_mode(m_supportDetails.presentModes))
            , m_extent(choose_extent(m_supportDetails.capabilities, width, height))
            , m_swapChain(create_swap_chain(m_windowSurface,
                                            m_logicalDevice,
                                            graphicsQueueIndex,
                                            presentationQueueIndex,
                                            m_supportDetails,
                                            m_surfaceFormat,
                                            m_presentationMode,
                                            m_extent,
                                            m_imagesCount)) {
            m_images = m_logicalDevice.getSwapchainImagesKHR(m_swapChain);
            m_imageViews = create_image_views(m_logicalDevice, m_surfaceFormat, m_images);
        }

        swap_chain::~swap_chain() {
            std::for_each(m_imageViews.begin(), m_imageViews.end(), [this](vk::ImageView& view) {
                m_logicalDevice.destroyImageView(view);
            });
            m_logicalDevice.destroySwapchainKHR(m_swapChain);
        }

        swap_chain::result swap_chain::acquire_next_image(const vk::Device& device, const vk::Semaphore& imageAvailable) {
            auto acquireResult = device.acquireNextImageKHR(
                m_swapChain, std::numeric_limits<uint64_t>::max(), imageAvailable, vk::Fence());

            if (acquireResult.result == vk::Result::eErrorOutOfDateKHR) {
                return result::resize;
            } else if (acquireResult.result != vk::Result::eSuccess &&
                       acquireResult.result != vk::Result::eSuboptimalKHR) {
                return result::fail;
            }
            m_currentImage = acquireResult.value;

            return result::success;
        }

        swap_chain::result swap_chain::present_image(const vk::Queue& presentationQueueHandle,
                                                     const vk::Semaphore& renderFinished) {
            vk::PresentInfoKHR presentInfo(1, &renderFinished, 1, &m_swapChain, &m_currentImage);

            auto presentResult = presentationQueueHandle.presentKHR(presentInfo);

            if (presentResult == vk::Result::eErrorOutOfDateKHR || presentResult == vk::Result::eSuboptimalKHR) {
                return result::resize;
            } else if (presentResult != vk::Result::eSuccess) {
                return result::fail;
            }
            return result::success;
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst

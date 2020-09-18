// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "swap_chain.h"

#include "physical_device.h"
#include "queue_indices.h"
#include "resources/image.h"
#include "resources/util.h"
#include "vulkan_exception.h"

#include <algorithm>

namespace tst {
namespace engine {

    namespace vulkan {

        namespace {

            vk::SurfaceFormatKHR choose_surface_format(const std::vector<vk::SurfaceFormatKHR>& availableFormats) {
                if (availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined) {
                    return {vk::Format::eB8G8R8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear};
                }

                for (const auto& availableFormat : availableFormats) {
                    if (availableFormat.format == vk::Format::eB8G8R8A8Srgb &&
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

            vk::Extent2D choose_extent(const vk::SurfaceCapabilitiesKHR& capabilities, const vk::Extent2D extent) {
                if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
                    return capabilities.currentExtent;
                } else {
                    vk::Extent2D actualExtent = extent;

                    actualExtent.width = std::clamp(
                        actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
                    actualExtent.height = std::clamp(
                        actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

                    return actualExtent;
                }
            }

            vk::SwapchainKHR create_swap_chain(const vk::SurfaceKHR windowSurface,
                                               const vk::Device logicalDevice,
                                               std::uint32_t graphicsQueueIndex,
                                               std::uint32_t presentationQueueIndex,
                                               const surface_support_info& supportInfo,
                                               const vk::SurfaceFormatKHR surfaceFormat,
                                               const vk::PresentModeKHR presentationMode,
                                               vk::Extent2D extent,
                                               settings::buffering buffers) {
                const auto buffersCount = static_cast<std::uint32_t>(buffers);
                if (supportInfo.capabilities.maxImageCount < buffersCount ||
                    buffersCount < supportInfo.capabilities.minImageCount) {
                    throw vulkan_exception("Requested not supported count of swap chain images");
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
                    windowSurface,
                    buffersCount,
                    surfaceFormat.format,
                    surfaceFormat.colorSpace,
                    extent,
                    1,
                    vk::ImageUsageFlagBits::eColorAttachment,
                    sharingMode,
                    queueFamilyIndexCount,
                    graphicsQueueIndex != presentationQueueIndex ? queueFamilyIndices : nullptr,
                    supportInfo.capabilities.currentTransform,
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

            std::vector<vk::ImageView> create_image_views(const vk::Device logicalDevice,
                                                          const vk::Format format,
                                                          const std::vector<vk::Image>& images) {
                std::vector<vk::ImageView> imageViews(images.size());

                for (std::uint32_t idx = 0; idx < imageViews.size(); idx++) {
                    imageViews[idx] =
                        create_image_view(logicalDevice, images[idx], format, vk::ImageAspectFlagBits::eColor);
                }
                return imageViews;
            }

            vk::Format find_supported_format(const physical_device& physicalDevice,
                                             const std::vector<vk::Format>& candidates,
                                             vk::ImageTiling tiling,
                                             vk::FormatFeatureFlags features) {
                for (const auto format : candidates) {
                    const auto props = physicalDevice.get_format_properties(format);

                    if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features) {
                        return format;
                    } else if (tiling == vk::ImageTiling::eOptimal &&
                               (props.optimalTilingFeatures & features) == features) {
                        return format;
                    }
                }

                throw std::runtime_error("failed to find supported format!");
            }

            vk::Format find_depth_format(const physical_device& physicalDevice) {
                return find_supported_format(
                    physicalDevice,
                    {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
                    vk::ImageTiling::eOptimal,
                    vk::FormatFeatureFlagBits::eDepthStencilAttachment);
            }

            [[maybe_unused]] bool has_stencil_component(vk::Format format) {
                return format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint;
            }

        } // namespace

        swap_chain::swap_chain(const physical_device& physicalDevice,
                               vk::Device device,
                               vk::SurfaceKHR windowSurface,
                               const surface_support_info& info,
                               std::uint32_t graphicsQueueIndex,
                               std::uint32_t presentationQueueIndex,
                               const vk::Extent2D extent,
                               settings::buffering buffers)
            : m_physicalDevice(physicalDevice)
            , m_logicalDevice(device)
            , m_windowSurface(windowSurface)
            , m_supportInfo(info)
            , m_surfaceFormat(choose_surface_format(m_supportInfo.formats))
            , m_presentationMode(choose_presentation_mode(m_supportInfo.presentModes))
            , m_extent(choose_extent(m_supportInfo.capabilities, extent))
            , m_buffering(buffers)
            , m_swapChain(create_swap_chain(m_windowSurface,
                                            m_logicalDevice,
                                            graphicsQueueIndex,
                                            presentationQueueIndex,
                                            m_supportInfo,
                                            m_surfaceFormat,
                                            m_presentationMode,
                                            m_extent,
                                            buffers))
            , m_depthFormat(find_depth_format(m_physicalDevice))
            , m_currentImage(0) {
            m_images = m_logicalDevice.getSwapchainImagesKHR(m_swapChain);
            m_imageViews = create_image_views(m_logicalDevice, m_surfaceFormat.format, m_images);
            std::tie(m_depthImage, m_depthImageMemory) = create_image(m_logicalDevice,
                                                                      m_extent,
                                                                      m_depthFormat,
                                                                      vk::ImageTiling::eOptimal,
                                                                      vk::ImageUsageFlagBits::eDepthStencilAttachment,
                                                                      m_physicalDevice.get_memory_properties(),
                                                                      vk::MemoryPropertyFlagBits::eDeviceLocal);
            m_depthImageView =
                create_image_view(m_logicalDevice, m_depthImage, m_depthFormat, vk::ImageAspectFlagBits::eDepth);
        }

        swap_chain::~swap_chain() {
            m_logicalDevice.destroyImageView(m_depthImageView);
            m_logicalDevice.destroyImage(m_depthImage);
            m_logicalDevice.freeMemory(m_depthImageMemory);
            std::for_each(m_imageViews.begin(), m_imageViews.end(), [this](vk::ImageView& view) {
                m_logicalDevice.destroyImageView(view);
            });
            m_logicalDevice.destroySwapchainKHR(m_swapChain);
        }

        swap_chain::result swap_chain::acquire_next_image(vk::Device device, const vk::Semaphore imageAvailable) {
            auto acquireResult = device.acquireNextImageKHR(
                m_swapChain, std::numeric_limits<std::uint64_t>::max(), imageAvailable, vk::Fence());

            if (acquireResult.result == vk::Result::eErrorOutOfDateKHR) {
                return result::resize;
            } else if (acquireResult.result != vk::Result::eSuccess &&
                       acquireResult.result != vk::Result::eSuboptimalKHR) {
                return result::fail;
            }
            m_currentImage = acquireResult.value;

            return result::success;
        }

        swap_chain::result swap_chain::present_image(vk::Queue presentationQueueHandle,
                                                     const vk::Semaphore renderFinished) {
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

// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "device.h"

#include "gpu_info.h"
#include "instance.h"
#include "resource_cache.h"
#include "swap_chain.h"
#include "vulkan_exception.h"

#include <GLFW/glfw3.h>
#include <algorithm/algorithm.h>
#include <application/app_event.h>
#include <application/data_loader.h>
#include <application/event_processor.h>
#include <application/main_window.h>
#include <fmt/printf.h>
#include <set>
#include <util/variant.h>

namespace tst {
namespace engine {
    namespace vulkan {

        vk::SurfaceKHR create_window_surface(GLFWwindow* window) {
            VkSurfaceKHR surface;
            if (vk::Result(glfwCreateWindowSurface(
                    instance::get_instance().get_instance_handle(), window, nullptr, &surface)) != vk::Result::eSuccess) {
                throw vulkan_exception("Failed to create window surface!");
            }
            return surface;
        }

        bool check_extensions_support(const vk::PhysicalDevice& handle,
                                      const std::vector<const char*>& requiredExtenstions) {
            const auto availableExtensions = handle.enumerateDeviceExtensionProperties();

            return tst::includes(availableExtensions.cbegin(),
                                 availableExtensions.cend(),
                                 requiredExtenstions.cbegin(),
                                 requiredExtenstions.cend(),
                                 [](const vk::ExtensionProperties availExtension, const char* reqExtension) {
                                     return std::string_view(availExtension.extensionName) ==
                                         std::string_view(reqExtension);
                                 });
        }

        std::uint32_t rate_device(gpu_info info) {
            std::uint32_t score = 0;

            switch (info.deviceType) {
            case gpu_info::device_type::discrete:
                score += 100;
                break;
            case gpu_info::device_type::integrated:
                score += 10;
                break;
            case gpu_info::device_type::cpu:
                score += 1;
                break;
            case gpu_info::device_type::other:
                break;
            }

            return score;
        }

        ptr<physical_device> select_physical_device(vk::SurfaceKHR& surface,
                                                    const std::vector<const char*>& requiredExtensions) {
            const auto& instance = instance::get_instance();
            std::vector<vk::PhysicalDevice> devices = instance.get_instance_handle().enumeratePhysicalDevices();

            if (devices.size() == 0) {
                throw vulkan_exception("Failed to find GPUs with Vulkan support!");
            }

            std::uint32_t maxScore = 0;
            ptr<physical_device> bestDevice;

            for (const auto& device : devices) {
                try {
                    if (!check_extensions_support(device, requiredExtensions)) {
                        throw vulkan_exception("Device is not supporting required extenstions");
                    }
                    auto indices = compute_queue_indices(surface, device);
                    ptr<gpu_info> info = std::make_unique<gpu_info>(device);

                    std::uint32_t score = rate_device(device);

                    if (maxScore < score) {
                        bestDevice = std::make_unique<physical_device>(device, std::move(info), indices);
                        maxScore = score;
                    }

                } catch (vulkan_exception& ex) {
                    fmt::printf("%s\n", ex.what());
                }
            }
            if (maxScore > 0) {
                return bestDevice;
            }

            throw vulkan_exception("Failed to find a suitable GPU!");
        }

        device::frame_resources::frame_resources(vk::Device device)
            : imageAvailable(device.createSemaphore(vk::SemaphoreCreateInfo()))
            , renderFinished(device.createSemaphore(vk::SemaphoreCreateInfo()))
            , inFlightFences(device.createFence(vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled))) {
        }

        device::device(application::main_window& mainWindow,
                       application::event_processor<application::app_event>& eventProcessor,
                       settings&& engineSettings)
            : m_frameCounter(0)
            , m_mainWindow(mainWindow)
            , m_eventProcessor(eventProcessor)
            , m_engineSettings(std::move(engineSettings))
            , m_windowSurface(create_window_surface(mainWindow.get_handle()))
            , m_physicalDevice(select_physical_device(m_windowSurface, {VK_KHR_SWAPCHAIN_EXTENSION_NAME}))
            , m_logicalDevice(m_physicalDevice->create_logical_device(instance::get_validation_layers(),
                                                                      {VK_KHR_SWAPCHAIN_EXTENSION_NAME}))
            , m_swapChain(
                  std::make_unique<swap_chain>(*m_physicalDevice.get(),
                                               m_logicalDevice,
                                               m_windowSurface,
                                               m_physicalDevice->get_surface_support_info(m_windowSurface),
                                               m_physicalDevice->get_graphics_index(),
                                               m_physicalDevice->get_presentation_index(),
                                               vk::Extent2D{mainWindow.get_size().width, mainWindow.get_size().height},
                                               m_engineSettings.m_buffering))
            , m_resourceCache(std::make_unique<resource_cache>(m_logicalDevice))
            , m_graphicsQueueHandle(m_logicalDevice.getQueue(m_physicalDevice->get_graphics_index(), 0))
            , m_computeQueueHandle(m_logicalDevice.getQueue(m_physicalDevice->get_compute_index(), 0))
            , m_presentationQueueHandle(m_logicalDevice.getQueue(m_physicalDevice->get_presentation_index(), 0))
            , m_transferQueueHandle(m_logicalDevice.getQueue(m_physicalDevice->get_transfer_index(), 0))
            , m_frameResources(
                  {frame_resources(m_logicalDevice), frame_resources(m_logicalDevice), frame_resources(m_logicalDevice)})
            , m_engineFrontend(std::make_unique<engine_frontend>(*this))
            , m_framebufferResizeInfo({false, {0, 0}})
            , m_resourceIndex(0) {
            auto framebufferResizeCallback = [&](const application::app_event::arguments& args) {
                assert(std::holds_alternative<application::app_event::framebuffer>(args));
                m_framebufferResizeInfo = {true, std::get<application::app_event::framebuffer>(args).size};
            };

            m_eventProcessor.subscribe(
                core::variant_index<application::app_event::arguments, application::app_event::framebuffer>(),
                this,
                std::move(framebufferResizeCallback));
        }

        device::~device() {
            for (auto& resource : m_frameResources) {
                m_logicalDevice.destroySemaphore(resource.imageAvailable);
                m_logicalDevice.destroySemaphore(resource.renderFinished);
                m_logicalDevice.destroyFence(resource.inFlightFences);
            }
            m_engineFrontend.reset();
            for (auto& commandPool : m_commandPools) {
                m_logicalDevice.destroyCommandPool(commandPool);
            }

            m_resourceCache->clear();
            m_swapChain.reset();
            instance::get_instance().m_instance.destroySurfaceKHR(m_windowSurface);
            m_logicalDevice.destroy();
        }

        resource_factory device::create_resource_factory(const application::data_loader& dataLoader) const {
            return resource_factory(*this, dataLoader);
        }

        vk::CommandPool device::create_command_pool() {
            vk::CommandPoolCreateInfo createInfo(vk::CommandPoolCreateFlags(), m_physicalDevice->get_graphics_index());

            m_commandPools.emplace_back(m_logicalDevice.createCommandPool(createInfo));

            return m_commandPools.back();
        }

        void device::start() {
        }

        void device::stop() {
            m_logicalDevice.waitIdle();
        }

        bool device::startFrame() {
            m_resourceIndex = m_frameCounter % settings::m_inFlightFrames;

            m_logicalDevice.waitForFences(
                1, &m_frameResources[m_resourceIndex].inFlightFences, true, std::numeric_limits<uint64_t>::max());

            auto acquireResult =
                m_swapChain->acquire_next_image(m_logicalDevice, m_frameResources[m_resourceIndex].imageAvailable);
            if (acquireResult == swap_chain::result::resize) {
                update_framebuffer();
                return false;
            } else if (acquireResult == swap_chain::result::fail) {
                return false;
            }
            return true;
        }

        bool device::draw(const std::vector<vk::CommandBuffer>& commandBuffers) {
            std::uint32_t currentFrame = get_resource_index();
            m_logicalDevice.resetFences(1, &m_frameResources[currentFrame].inFlightFences);

            vk::Semaphore waitSemaphores[] = {m_frameResources[currentFrame].imageAvailable};
            vk::Semaphore signalSemaphores[] = {m_frameResources[currentFrame].renderFinished};
            vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};

            vk::SubmitInfo submitInfo(1,
                                      waitSemaphores,
                                      waitStages,
                                      static_cast<std::uint32_t>(commandBuffers.size()),
                                      commandBuffers.data(),
                                      1,
                                      signalSemaphores);

            m_graphicsQueueHandle.submit(1, &submitInfo, m_frameResources[currentFrame].inFlightFences);

            return true;
        }

        bool device::endFrame() {
            std::uint32_t currentFrame = get_resource_index();
            ++m_frameCounter;

            auto presentResult =
                m_swapChain->present_image(m_presentationQueueHandle, m_frameResources[currentFrame].renderFinished);
            if (m_framebufferResizeInfo.shouldResize || presentResult == swap_chain::result::resize) {
                update_framebuffer();
                return true;
            } else if (presentResult == swap_chain::result::fail) {
                return false;
            }
            return true;
        }

        void device::update_framebuffer() {
            m_framebufferResizeInfo.shouldResize = false;
            m_mainWindow.set_size(m_framebufferResizeInfo.size);
            recreate_swap_chain(m_framebufferResizeInfo.size);
        }

        void device::recreate_swap_chain(const core::extent<std::uint32_t>& extent) {
            m_logicalDevice.waitIdle();

            for (auto& resource : m_frameResources) {
                m_logicalDevice.destroySemaphore(resource.imageAvailable);
                m_logicalDevice.destroySemaphore(resource.renderFinished);
                m_logicalDevice.destroyFence(resource.inFlightFences);
            }

            m_frameResources = {
                frame_resources(m_logicalDevice), frame_resources(m_logicalDevice), frame_resources(m_logicalDevice)};

            m_swapChain.reset();

            auto newSwapChain = std::make_unique<swap_chain>(*m_physicalDevice.get(),
                                                             m_logicalDevice,
                                                             m_windowSurface,
                                                             m_physicalDevice->get_surface_support_info(m_windowSurface),
                                                             m_physicalDevice->get_graphics_index(),
                                                             m_physicalDevice->get_presentation_index(),
                                                             vk::Extent2D{extent.width, extent.height},
                                                             m_engineSettings.m_buffering);

            m_swapChain = std::move(newSwapChain);

            m_resourceCache->rebuild_techniques(*m_swapChain.get());
            m_resourceCache->rebuild_pipelines();
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst

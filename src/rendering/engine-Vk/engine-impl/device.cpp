// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "device.h"

#include "gpu_info.h"
#include "instance.h"
#include "swap_chain.h"
#include "vulkan_exception.h"

#include <GLFW/glfw3.h>
#include <algorithm/algorithm.h>
#include <application/app_event.h>
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
            auto availableExtensions = handle.enumerateDeviceExtensionProperties();

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
            auto& instance = instance::get_instance();
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

        device::device(application::main_window& mainWindow,
                       application::event_processor<application::app_event>& eventProcessor)
            : m_frameCounter(0)
            , m_mainWindow(mainWindow)
            , m_eventProcessor(eventProcessor)
            , m_windowSurface(create_window_surface(mainWindow.get_handle()))
            , m_physicalDevice(select_physical_device(m_windowSurface, {VK_KHR_SWAPCHAIN_EXTENSION_NAME}))
            , m_logicalDevice(m_physicalDevice->create_logical_device(instance::get_validation_layers(),
                                                                      {VK_KHR_SWAPCHAIN_EXTENSION_NAME}))
            , m_swapChain(std::make_unique<swap_chain>(m_logicalDevice,
                                                       m_windowSurface,
                                                       m_physicalDevice->get_surface_support_info(m_windowSurface),
                                                       m_physicalDevice->get_graphics_index(),
                                                       m_physicalDevice->get_presentation_index(),
                                                       mainWindow.get_size().width,
                                                       mainWindow.get_size().height))
            , m_graphicsQueueHandle(m_logicalDevice.getQueue(m_physicalDevice->get_graphics_index(), 0))
            , m_computeQueueHandle(m_logicalDevice.getQueue(m_physicalDevice->get_compute_index(), 0))
            , m_presentationQueueHandle(m_logicalDevice.getQueue(m_physicalDevice->get_presentation_index(), 0))
            , m_transferQueueHandle(m_logicalDevice.getQueue(m_physicalDevice->get_transfer_index(), 0))
            , m_imageAvailable({m_logicalDevice.createSemaphore(vk::SemaphoreCreateInfo()),
                                m_logicalDevice.createSemaphore(vk::SemaphoreCreateInfo())})
            , m_renderFinished({m_logicalDevice.createSemaphore(vk::SemaphoreCreateInfo()),
                                m_logicalDevice.createSemaphore(vk::SemaphoreCreateInfo())})
            , m_inFlightFences({m_logicalDevice.createFence(vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled)),
                                m_logicalDevice.createFence(vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled))})
            , m_framebufferResized(false) {
            auto framebufferResizeCallback = [&](const application::app_event::arguments&) {
                m_framebufferResized = true;
            };

            m_eventProcessor.subscribe(
                core::variant_index<application::app_event::arguments, application::app_event::framebuffer>(),
                this,
                std::move(framebufferResizeCallback));
        }

        device::~device() {
            for (auto& semaphore : m_imageAvailable) {
                m_logicalDevice.destroySemaphore(semaphore);
            }
            for (auto& semaphore : m_renderFinished) {
                m_logicalDevice.destroySemaphore(semaphore);
            }
            for (auto& fence : m_inFlightFences) {
                m_logicalDevice.destroyFence(fence);
            }

            instance::get_instance().m_instance.destroySurfaceKHR(m_windowSurface);

            for (auto& commandPool : m_commandPools) {
                m_logicalDevice.destroyCommandPool(commandPool);
            }

            m_logicalDevice.destroy();
        }

        vk::CommandPool& device::create_command_pool() {
            vk::CommandPoolCreateInfo createInfo(vk::CommandPoolCreateFlags(), m_physicalDevice->get_graphics_index());

            m_commandPools.push_back(m_logicalDevice.createCommandPool(createInfo));

            return m_commandPools.back();
        }

        vertex_buffer device::create_vertex_buffer(const vertex_format& format,
                                                   std::vector<vertex>&& vertices,
                                                   const vk::CommandPool& cmdPool) const {
            return vertex_buffer(m_logicalDevice,
                                 m_graphicsQueueHandle,
                                 cmdPool,
                                 m_physicalDevice->get_memory_properties(),
                                 format,
                                 std::move(vertices));
        }

        uniform_buffer device::create_uniform_buffer(const vk::CommandPool& cmdPool) const {
            return uniform_buffer(
                m_logicalDevice, m_graphicsQueueHandle, cmdPool, m_physicalDevice->get_memory_properties());
        }

        shader device::crate_shader(shader::shader_type type,
                                    std::vector<char>&& source,
                                    const std::string_view name) const {
            return shader(m_logicalDevice, type, std::move(source), name);
        }

        bool device::startFrame() {
            std::uint32_t currentSemaphore = m_frameCounter % m_maxConcurrentFrames;

            m_logicalDevice.waitForFences(
                1, &m_inFlightFences[currentSemaphore], true, std::numeric_limits<uint64_t>::max());

            auto acquireResult = m_swapChain->acquire_next_image(m_logicalDevice, m_imageAvailable[currentSemaphore]);
            if (acquireResult == swap_chain::result::resize) {
                // recreate swap chain
                return false;
            } else if (acquireResult == swap_chain::result::fail) {
                return false;
            }
            return true;
        }

        bool device::draw(const std::vector<vk::CommandBuffer>& commandBuffers) {
            std::uint32_t currentSemaphore = m_frameCounter % m_maxConcurrentFrames;

            m_logicalDevice.resetFences(1, &m_inFlightFences[currentSemaphore]);

            vk::Semaphore waitSemaphores[] = {m_imageAvailable[currentSemaphore]};
            vk::Semaphore signalSemaphores[] = {m_renderFinished[currentSemaphore]};
            vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};

            vk::SubmitInfo submitInfo(1, waitSemaphores, waitStages, 1, commandBuffers.data(), 1, signalSemaphores);

            m_graphicsQueueHandle.submit(1, &submitInfo, m_inFlightFences[currentSemaphore]);

            return true;
        }

        bool device::endFrame() {
            std::uint32_t currentSemaphore = m_frameCounter % m_maxConcurrentFrames;

            auto presentResult =
                m_swapChain->present_image(m_presentationQueueHandle, m_renderFinished[currentSemaphore]);
            if (m_framebufferResized || presentResult == swap_chain::result::resize) {
                return true;
            } else if (presentResult == swap_chain::result::fail) {
                return false;
            }
            return true;
        }

        void device::update_framebuffer() {
            m_framebufferResized = false;

            std::int32_t width, height;
            glfwGetFramebufferSize(m_mainWindow.get_handle(), &width, &height);
            m_mainWindow.set_size({width, height});
            recreate_swap_chain(width, height);
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst

// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "device.h"

#include "gpu_info.h"
#include "instance.h"
#include "swap_chain.h"
#include "vulkan_exception.h"

#include <GLFW/glfw3.h>
#include <algorithm/algorithm.h>
#include <application/main_window.h>
#include <fmt/printf.h>
#include <set>

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

        vk::PhysicalDevice select_physical_device(vk::SurfaceKHR& surface,
                                                  const std::vector<const char*>& requiredExtensions) {
            auto& instance = instance::get_instance();
            std::vector<vk::PhysicalDevice> devices = instance.get_instance_handle().enumeratePhysicalDevices();

            if (devices.size() == 0) {
                throw vulkan_exception("Failed to find GPUs with Vulkan support!");
            }

            for (const auto& device : devices) {
                try {
                    if (!check_extensions_support(device, requiredExtensions)) {
                        throw vulkan_exception("Device is not supporting required extenstions");
                    }

                    compute_queue_indices(surface, device);

                    return device;

                } catch (vulkan_exception& ex) {
                    fmt::printf("%s\n", ex.what());
                }
            }

            throw vulkan_exception("Failed to find a suitable GPU!");
        }

        vk::Device create_logical_device(const vk::PhysicalDevice& physicalDevice,
                                         const queue_family_indices& indices,
                                         const std::vector<const char*>& validationLayers,
                                         const std::vector<const char*>& extensions) {
            std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

            std::set<std::uint32_t> uniqueQueueFamilies = {indices.graphicsIndex.value(),
                                                           indices.presentationIndex.value(),
                                                           indices.computeIndex.value(),
                                                           indices.transferIndex.value()};

            float queuePriority = 1.0f;
            for (uint32_t queueFamily : uniqueQueueFamilies) {
                vk::DeviceQueueCreateInfo queueCreateInfo(vk::DeviceQueueCreateFlags(), queueFamily, 1, &queuePriority);
                queueCreateInfos.push_back(queueCreateInfo);
            }

            vk::DeviceCreateInfo createInfo(vk::DeviceCreateFlags(),
                                            static_cast<uint32_t>(queueCreateInfos.size()),
                                            queueCreateInfos.data(),
                                            static_cast<uint32_t>(validationLayers.size()),
                                            validationLayers.data(),
                                            static_cast<uint32_t>(extensions.size()),
                                            extensions.data(),
                                            nullptr);

            try {
                return physicalDevice.createDevice(createInfo);
            } catch (std::runtime_error&) {
                throw vulkan_exception("Failed to create logical device!");
            }
        }

        device::device(application::main_window& mainWindow)
            : m_windowSurface(create_window_surface(mainWindow.get_handle()))
            , m_physicalDevice(select_physical_device(m_windowSurface, {VK_KHR_SWAPCHAIN_EXTENSION_NAME}))
            , m_gpuInfo(std::make_unique<gpu_info>(m_physicalDevice))
            , m_queueIndices(compute_queue_indices(m_windowSurface, m_physicalDevice))
            , m_logicalDevice(create_logical_device(
                  m_physicalDevice, m_queueIndices, instance::get_validation_layers(), {VK_KHR_SWAPCHAIN_EXTENSION_NAME}))
            , m_swapChain(std::make_unique<vulkan::swap_chain>(
                  m_logicalDevice, m_physicalDevice, m_windowSurface, m_queueIndices, mainWindow.get_size().width, mainWindow.get_size().height))
            , m_graphicsQueueHandle(m_logicalDevice.getQueue(m_queueIndices.graphicsIndex.value(), 0))
            , m_computeQueueHandle(m_logicalDevice.getQueue(m_queueIndices.computeIndex.value(), 0))
            , m_presentationQueueHandle(m_logicalDevice.getQueue(m_queueIndices.presentationIndex.value(), 0))
            , m_transferQueueHandle(m_logicalDevice.getQueue(m_queueIndices.transferIndex.value(), 0)) {
        }

        device::~device() {
            instance::get_instance().m_instance.destroySurfaceKHR(m_windowSurface);

            for (auto& commandPool : m_commandPools) {
                m_logicalDevice.destroyCommandPool(commandPool);
            }

            m_logicalDevice.destroy();
        }

        vk::CommandPool& device::create_command_pool() {
            vk::CommandPoolCreateInfo createInfo(vk::CommandPoolCreateFlags(), m_queueIndices.graphicsIndex.value());

            m_commandPools.push_back(m_logicalDevice.createCommandPool(createInfo));

            return m_commandPools.back();
        }

        vertex_buffer device::create_vertex_buffer(const vertex_format& format,
                                                   std::vector<vertex>&& vertices,
                                                   const vk::CommandPool& cmdPool) const {
            return vertex_buffer(
                m_logicalDevice, m_physicalDevice, m_graphicsQueueHandle, cmdPool, format, std::move(vertices));
        }

        uniform_buffer device::create_uniform_buffer(const vk::CommandPool& cmdPool) const {
            return uniform_buffer(m_logicalDevice, m_physicalDevice, m_graphicsQueueHandle, cmdPool);
        }

        shader device::crate_shader(shader::shader_type type,
                                    std::vector<char>&& source,
                                    const std::string_view name) const {
            return shader(m_logicalDevice, type, std::move(source), name);
        }

        void device::startFrame() {

        }

        void device::draw() {

        }

        void device::endFrame() {
            
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst

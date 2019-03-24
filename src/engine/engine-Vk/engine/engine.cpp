// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "engine.h"

#include "device_queue.h"
#include "physical_device.h"
#include "scene/scene.h"
#include "swap_chain.h"
#include "vulkan_exception.h"

#include <application/main_window.h>

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

namespace tst {
namespace engine {

    static std::vector<const char*> generateLayers() {
        if (enableValidationLayers) {
            return {"VK_LAYER_LUNARG_standard_validation"};
        } else
            return {};
    }

    rendering_engine::rendering_engine(application::data_loader& dataLoader, application::main_window& mainWindow)
        : m_dataLoader(dataLoader)
        , m_scene(std::make_unique<scene::scene>())
        , m_requiredValidationLayers(generateLayers())
        , m_vulkanInstance(vulkan::init_Vulkan_instance(m_requiredValidationLayers, enableValidationLayers))
        , m_debugMessenger(vulkan::setup_debug_messenger(m_vulkanInstance, enableValidationLayers))
        , m_windowSurface(vulkan::create_window_surface(m_vulkanInstance, mainWindow.get_handle()))
        , m_physicalDevice(vulkan::select_physical_device(m_vulkanInstance, m_windowSurface, m_reqiuredDeviceExtensions))
        , m_queueIndices(vulkan::compute_queue_indices(m_physicalDevice, m_windowSurface))
        , m_logicalDevice(vulkan::create_logical_device(
              m_physicalDevice, m_queueIndices, m_requiredValidationLayers, m_reqiuredDeviceExtensions))
        , m_deviceQueues(std::make_unique<vulkan::device_queues>(m_logicalDevice, m_queueIndices))
        , m_swapChain(std::make_unique<vulkan::swap_chain>(m_physicalDevice,
                                                           m_logicalDevice,
                                                           m_windowSurface,
                                                           m_queueIndices,
                                                           mainWindow.get_size().width,
                                                           mainWindow.get_size().height)) {
    }

    rendering_engine::~rendering_engine() {
        if (enableValidationLayers) {
            vulkan::DestroyDebugUtilsMessengerEXT(m_vulkanInstance, m_debugMessenger, nullptr);
        }
        m_swapChain.reset();
        m_vulkanInstance.destroySurfaceKHR(m_windowSurface);
        m_logicalDevice.destroy();
        m_vulkanInstance.destroy();
    }

    void rendering_engine::frame() {
    }

} // namespace engine
} // namespace tst
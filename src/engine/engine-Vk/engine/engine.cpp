// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "engine.h"

#include "scene/scene.h"
#include "vulkan_exception.h"

#include <application/main_window.h>

namespace tst {
namespace engine {

    rendering_engine::rendering_engine(application::data_loader& dataLoader, application::main_window& mainWindow)
        : m_dataLoader(dataLoader)
        , m_scene(std::make_unique<scene::scene>())
        , m_vulkanInstance(vulkan::init_Vulkan_instance(m_requiredValidationLayers))
        , m_debugMessenger(vulkan::setup_debug_messenger(m_vulkanInstance))
        , m_windowSurface(vulkan::create_window_surface(m_vulkanInstance, mainWindow.get_handle()))
        , m_physicalDevice(vulkan::select_physical_device(m_vulkanInstance, m_windowSurface, m_reqiuredDeviceExtensions))
        , m_logicalDevice(
              vulkan::create_logical_device(m_physicalDevice, m_requiredValidationLayers, m_reqiuredDeviceExtensions))
        , m_graphicsQueue(
              vulkan::create_queue(m_logicalDevice, m_physicalDevice.get_queue_family_indices().graphicsIndex.value()))
        , m_presentationQueue(vulkan::create_queue(
              m_logicalDevice, m_physicalDevice.get_queue_family_indices().presentationIndex.value())) {
    }

    rendering_engine::~rendering_engine() {
        if (enableValidationLayers) {
            vulkan::DestroyDebugUtilsMessengerEXT(m_vulkanInstance, m_debugMessenger, nullptr);
        }
        vkDestroyDevice(m_logicalDevice, nullptr);
        vkDestroySurfaceKHR(m_vulkanInstance, m_windowSurface, nullptr);
        vkDestroyInstance(m_vulkanInstance, nullptr);
    }

    void rendering_engine::frame() {
    }

} // namespace engine
} // namespace tst
// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "engine.h"
#include "vulkan_exception.h"

#include "scene/scene.h"

namespace tst {
namespace engine {

    rendering_engine::rendering_engine(application::data_loader& dataLoader)
        : m_dataLoader(dataLoader), m_scene(std::make_unique<scene::scene>()) {
        vulkan::setup_validation_layers(m_requiredValidationLayers);
        m_vulkanInstance = vulkan::init_Vulkan_instance(m_requiredValidationLayers);
        m_debugMessenger = vulkan::setup_debug_messenger(m_vulkanInstance);
        m_physicalDevice = vulkan::select_physical_device(m_vulkanInstance);
        m_families = vulkan::find_queue_families(m_physicalDevice);
        m_logicalDevice = vulkan::create_logical_device(m_physicalDevice, m_requiredValidationLayers);
        m_graphicsQueue = vulkan::create_graphics_queue(m_logicalDevice, m_families);
    }

    rendering_engine::~rendering_engine() {
        if (enableValidationLayers) {
            vulkan::DestroyDebugUtilsMessengerEXT(m_vulkanInstance, m_debugMessenger, nullptr);
        }
        vkDestroyDevice(m_logicalDevice, nullptr);
        vkDestroyInstance(m_vulkanInstance, nullptr);
    }

    void rendering_engine::frame() {}

} // namespace engine
} // namespace tst
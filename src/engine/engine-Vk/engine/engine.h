// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "engine_init.h"

#include <vulkan/vulkan.h>

#include <memory>
#include <vector>

namespace tst {
namespace application {
    class data_loader;
}

namespace scene {
    class scene;
}

namespace engine {

    class rendering_engine {
        template<typename T>
        using ptr = std::unique_ptr<T>;

    public:
        rendering_engine(application::data_loader& dataLoader);
        ~rendering_engine();

        void frame();
        void start();
        void stop();

    private:

        application::data_loader& m_dataLoader;
        ptr<scene::scene> m_scene;
        std::vector<const char*> m_requiredValidationLayers = {"VK_LAYER_LUNARG_standard_validation"};

        VkInstance m_vulkanInstance;
        VkDebugUtilsMessengerEXT m_debugMessenger;
        VkPhysicalDevice m_physicalDevice;
        vulkan::device_queue_families m_families;
        VkDevice m_logicalDevice;
        VkQueue m_graphicsQueue;
    };

} // namespace engine
} // namespace tst
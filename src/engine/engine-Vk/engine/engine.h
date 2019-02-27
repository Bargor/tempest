// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <vulkan/vulkan.h>

#include <memory>

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
        void init_Vulkan();

        application::data_loader& m_dataLoader;
        ptr<scene::scene> m_scene;
        VkInstance m_vulkanInstance;
    };

} // namespace engine
} // namespace tst
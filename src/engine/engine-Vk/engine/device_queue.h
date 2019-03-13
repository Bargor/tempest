// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <vulkan/vulkan.h>

#include <cstdint>

namespace tst {
namespace engine {

    namespace vulkan {

        class logical_device;
    
        class device_queue {
        public:
            device_queue(logical_device& device, std::uint32_t queueFamilyIndex);

            ~device_queue();
        private:
            VkQueue m_queueHandle;
        };

    }
} // namespace engine
} // namespace tst
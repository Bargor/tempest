// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "device.h"
#include "resources/index_buffer.h"

#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {

        class engine_frontend {
        public:
            engine_frontend(device& device);

            vk::CommandBuffer create_command_buffer();

        private:
            device& m_device;
            vk::CommandPool& m_commandPool;
        };
    } // namespace vulkan

} // namespace engine
} // namespace tst
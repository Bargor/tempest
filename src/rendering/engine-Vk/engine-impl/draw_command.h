// This file is part of Tempest project
// Author: Karol Kontny
#pragma once

#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {
        class draw_command {
        public:
        private:
            vk::CommandBuffer m_buffer;
        };
    } // namespace vulkan
} // namespace engine
} // namespace tst
// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-base/gpu_info.h>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {
        class gpu_info : public base::gpu_info {
            using super = base::gpu_info;
        public:

            gpu_info(vk::PhysicalDevice physicalDevice);

        private:
            std::uint32_t maxPushConstantSize;
            std::uint32_t maxMemoryAllocationCount;
        };
    } // namespace vulkan
} // namespace engine
} // namespace tst

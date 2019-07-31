// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-base/gpu_info.h>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {
        class gpu_info : public base::gpu_info {
            public:
                using super = base::gpu_info;

                gpu_info(const vk::PhysicalDevice& physicalDevice);
            private:
        };
    }
}
}
// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {

		class device;

		class engine_frontend {
        private:
            engine_frontend(device& device);
		public:
            device& m_device;
            vk::CommandPool m_commandPool;
		};

	}

} // namespace engine
} // namespace tst
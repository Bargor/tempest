// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "buffer.h"
#include <glm/glm.hpp>

#include <vector>

namespace tst {
namespace engine {
    namespace vulkan {

		struct uniform_buffer_object {
            glm::mat4 model;
            glm::mat4 view;
            glm::mat4 proj;
        };

        class uniform_buffer : public buffer {
        public:
            uniform_buffer(const device& device,
                          const vk::CommandPool& cmdPool);

			uniform_buffer(uniform_buffer&& other) noexcept;
            uniform_buffer(const uniform_buffer& other) = delete;

			~uniform_buffer();

			void update_buffer(const uniform_buffer_object& ubo);

        private:
            uniform_buffer_object m_data;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
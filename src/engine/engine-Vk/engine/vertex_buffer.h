// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "vertex_format.h"

#include <vector>

namespace tst {
namespace engine {
    namespace vulkan {

        class vertex_buffer {
        public:
            vertex_buffer(const vk::Device& device, const vertex_format& format, std::vector<vertex>&& vertices);
            ~vertex_buffer();

        private:
            const vk::Device& m_device;
            vertex_format m_format;
            std::vector<vertex> m_vertices;
            vk::Buffer m_buffer;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
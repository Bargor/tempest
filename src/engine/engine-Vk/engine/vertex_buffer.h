// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "buffer.h"
#include "vertex_format.h"

#include <vector>

namespace tst {
namespace engine {
    namespace vulkan {

        class vertex_buffer : public buffer {
        public:
            vertex_buffer(device& device,
                          vk::CommandPool& cmdPool,
                          const vertex_format& format,
                          std::vector<vertex>&& vertices);
            ~vertex_buffer();

            std::uint32_t get_vertex_count() const;

        private:
        private:
            vertex_format m_format;
            std::vector<vertex> m_vertices;
        };

        inline std::uint32_t vertex_buffer::get_vertex_count() const {
            return static_cast<std::uint32_t>(m_vertices.size());
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
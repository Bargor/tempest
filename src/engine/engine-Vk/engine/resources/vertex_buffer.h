// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "buffer.h"
#include "vk_vertex_format.h"

#include <vector>

namespace tst {
namespace engine {
    namespace vulkan {

        class vertex_buffer : public buffer {
        public:
            vertex_buffer(const vk::Device& logicalDevice,
                          const vk::PhysicalDevice& m_physicallDevice,
                          const vk::Queue m_queueHandle,
                          const vk::CommandPool& cmdPool,
                          const vertex_format& format,
                          std::vector<vertex>&& vertices);
            ~vertex_buffer();

            vertex_buffer(vertex_buffer&& other) noexcept;

            std::uint32_t get_vertex_count() const;

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
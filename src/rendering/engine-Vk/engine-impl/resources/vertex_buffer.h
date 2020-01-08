// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "buffer.h"
#include "vertex_format.h"

#include <vector>

namespace tst {
namespace engine {
    namespace vulkan {

        struct vertex_buffer_draw_info {
            vk::Buffer buffer;
            std::uint32_t count;
        };

        class vertex_buffer : public buffer {
            using super = buffer;
        public:

            vertex_buffer(const vk::Device logicalDevice,
                          const vk::Queue m_queueHandle,
                          const vk::CommandPool cmdPool,
                          const vk::PhysicalDeviceMemoryProperties memoryProperties,
                          const vertex_format& format,
                          std::vector<vertex>&& vertices);
            ~vertex_buffer();

            vertex_buffer(vertex_buffer&& other) noexcept;

        public:

            std::uint32_t get_vertex_count() const noexcept;

            const vertex_format& get_vertex_format() const noexcept;

        private:
            const vertex_format m_format;
            std::vector<vertex> m_vertices;
        };

        inline std::uint32_t vertex_buffer::get_vertex_count() const noexcept {
            return static_cast<std::uint32_t>(m_vertices.size());
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst

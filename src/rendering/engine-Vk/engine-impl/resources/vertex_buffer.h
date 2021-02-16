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
            using super = buffer;
        public:

            vertex_buffer(const buffer::creation_info& info,
                          const vertex_format& format,
                          std::vector<vertex>&& vertices);
            ~vertex_buffer();

            vertex_buffer(vertex_buffer&& other) noexcept;
            vertex_buffer(const vertex_buffer& other) = delete;

            vertex_buffer& operator=(vertex_buffer&& other);

        public:
            std::uint32_t get_vertex_count() const noexcept;

            const vertex_format& get_vertex_format() const noexcept;

        private:
            vertex_format m_format;
            std::vector<vertex> m_vertices;
        };

        TST_INLINE std::uint32_t vertex_buffer::get_vertex_count() const noexcept {
            return static_cast<std::uint32_t>(m_vertices.size());
        }

        TST_INLINE const vertex_format& vertex_buffer::get_vertex_format() const noexcept {
            return m_format;
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst

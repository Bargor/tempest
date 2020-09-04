// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "vertex_format.h"

#include "buffer.h"

namespace tst {
namespace engine {
    namespace opengl {
        class vertex_buffer : public buffer {
        public:
            vertex_buffer(const buffer_construction_info& info, const vertex_format& format,
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
	}
}
}
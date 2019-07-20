// This file is part of Tempest-engine-GL project
// Author: Karol Kontny
#pragma once

#include <engine/api.h>
#include <engine/resources/vertex_buffer.h>
#include <engine/resources/vertex_format.h>

namespace tst {
namespace engine {
    namespace resources {

        class vertex_buffer {
        public:
            vertex_buffer(api::vertex_buffer&& nativeBuffer);
            ~vertex_buffer();

            vertex_buffer(vertex_buffer&& other) noexcept;

        private:
            api::vertex_buffer m_nativeVertexBuffer;
        };
    } // namespace resources
} // namespace engine
} // namespace tst
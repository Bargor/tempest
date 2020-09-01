// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "index_buffer.h"

namespace tst {
namespace engine {
    namespace opengl {

        index_buffer::index_buffer(index_buffer&& other) noexcept
            : buffer(std::move(other)), m_indices(std::move(other.m_indices)) {
        }

        index_buffer::~index_buffer() {
        }

    } // namespace opengl
} // namespace engine
} // namespace tst
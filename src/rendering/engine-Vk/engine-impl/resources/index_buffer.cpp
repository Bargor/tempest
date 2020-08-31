// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "index_buffer.h"

namespace tst {
namespace engine {
    namespace vulkan {

        index_buffer::index_buffer(index_buffer&& other) noexcept
            : buffer(std::move(other)), m_format(other.m_format), m_indices(std::move(other.m_indices)) {
        }
    } // namespace vulkan
} // namespace engine
} // namespace tst
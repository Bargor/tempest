// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "index_buffer.h"

namespace tst {
namespace engine {
    namespace resources {

        index_buffer::index_buffer(std::vector<std::uint16_t>&& indices) : m_nativeIndexBuffer(std::move(indices)) {
        }

        index_buffer::~index_buffer() {
        }

    } // namespace resources
} // namespace engine
} // namespace tst
// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "index_buffer.h"

namespace tst {
namespace engine {
    namespace resources {

        index_buffer::index_buffer(api::index_buffer<std::uint16_t>&& nativeBuffer)
            : m_nativeIndexBuffer(std::move(nativeBuffer)) {
        }

        index_buffer::~index_buffer() {
        }

        index_buffer::index_buffer(index_buffer&& other) noexcept : m_nativeIndexBuffer(std::move(other.m_nativeIndexBuffer)) {
        }

    } // namespace resources
} // namespace engine
} // namespace tst
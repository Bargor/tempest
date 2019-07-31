// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "index_buffer.h"

namespace tst {
namespace engine {
    namespace resources {

        index_buffer::index_buffer(api::index_buffer<std::uint16_t>&& bufferImpl)
            : api::index_buffer<std::uint16_t>(std::move(bufferImpl)) {
        }

        index_buffer::~index_buffer() {
        }

        index_buffer::index_buffer(index_buffer&& other) noexcept
            : api::index_buffer<std::uint16_t>(std::move(other)) {
        }

    } // namespace resources
} // namespace engine
} // namespace tst
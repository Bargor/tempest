// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "index_buffer.h"

namespace tst {
namespace engine {
    namespace resources {

        index_buffer::index_buffer(api::index_buffer&& bufferImpl) : api::index_buffer(std::move(bufferImpl)) {
        }

        index_buffer::~index_buffer() {
        }

        index_buffer::index_buffer(index_buffer&& other) noexcept : api::index_buffer(std::move(other)) {
        }

    } // namespace resources
} // namespace engine
} // namespace tst
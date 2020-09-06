// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "index_buffer.h"

namespace tst {
namespace engine {
    namespace resources {

        index_buffer::index_buffer(const creation_info& info, std::vector<std::uint16_t>&& indices)
            : api::index_buffer(info, std::move(indices)) {
        }

        index_buffer::index_buffer(const creation_info& info, std::vector<std::uint32_t>&& indices)
            : api::index_buffer(info, std::move(indices)) {
        }

        index_buffer::index_buffer(index_buffer&& other) noexcept : api::index_buffer(std::move(other)) {
        }

        index_buffer::~index_buffer() {
        }

    } // namespace resources
} // namespace engine
} // namespace tst
// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "uniform_buffer.h"

namespace tst {
namespace engine {
    namespace resources {

        uniform_buffer::uniform_buffer(const creation_info& info, std::uint32_t binding, std::size_t storageSize)
            : api::uniform_buffer(info, binding, storageSize) {
        }

        uniform_buffer::uniform_buffer(uniform_buffer&& other) noexcept : api::uniform_buffer(std::move(other)) {
        }

    } // namespace resources
} // namespace engine
} // namespace tst
// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "uniform_buffer.h"

namespace tst {
namespace engine {
    namespace resources {

        uniform_buffer::uniform_buffer(api::uniform_buffer&& bufferImpl) : api::uniform_buffer(std::move(bufferImpl)) {
        }

        uniform_buffer::uniform_buffer(uniform_buffer&& other) noexcept : api::uniform_buffer(std::move(other)) {
        }

    } // namespace resources
} // namespace engine
} // namespace tst
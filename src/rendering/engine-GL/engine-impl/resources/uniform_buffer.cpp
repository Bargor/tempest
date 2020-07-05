// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "uniform_buffer.h"

namespace tst {
namespace engine {
    namespace opengl {
        uniform_buffer::uniform_buffer(const std::size_t storageSize)
            : buffer(storageSize, nullptr, GL_STREAM_DRAW) {
        }

        uniform_buffer::~uniform_buffer() {
        }

        uniform_buffer::uniform_buffer(uniform_buffer&& other) noexcept : buffer(std::move(other)) {
        }

        void uniform_buffer::update_buffer(const void*, const std::size_t) {
        }
    } // namespace opengl
} // namespace engine
} // namespace tst
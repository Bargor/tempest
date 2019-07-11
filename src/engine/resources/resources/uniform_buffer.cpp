// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "uniform_buffer.h"

namespace tst {
namespace engine {
    namespace resources {

        uniform_buffer::uniform_buffer(api::uniform_buffer&& nativeBuffer)
            : m_nativeUniformBuffer(std::move(nativeBuffer)) {
        }

		uniform_buffer::~uniform_buffer() {
        }

    } // namespace resources
} // namespace engine
} // namespace tst
// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-impl/api.h>
#include <engine-impl/resources/index_buffer.h>

namespace tst {
namespace engine {

    namespace resources {

        class index_buffer {
        public:
            index_buffer(api::index_buffer<std::uint16_t>&& bufferImpl);
            ~index_buffer();

            index_buffer(index_buffer&& other) noexcept;

        private:
            api::index_buffer<std::uint16_t> m_indexBufferImpl;
        };

    } // namespace resources
} // namespace engine
} // namespace tst
// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine/api.h>
#include <engine/resources/index_buffer.h>

namespace tst {
namespace engine {

    namespace resources {

        class index_buffer {
        public:
            index_buffer(api::index_buffer<std::uint16_t>&& nativeBuffer);
            ~index_buffer();

        private:
            api::index_buffer<std::uint16_t> m_nativeIndexBuffer;
        };

    } // namespace resources
} // namespace engine
} // namespace tst
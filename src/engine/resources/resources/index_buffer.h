// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine/api.h>
#include <engine/resources/index_buffer.h>

namespace tst {
namespace engine {

	namespace api {
        class engine_frontend;
	}

    namespace resources {

        class index_buffer {
        public:
            index_buffer(std::vector<std::uint16_t>&& indices);
            ~index_buffer();

        private:
            api::index_buffer<std::uint16_t> m_nativeIndexBuffer;
        };

    } // namespace resources
} // namespace engine
} // namespace tst
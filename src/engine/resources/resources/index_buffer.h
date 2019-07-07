// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine/resources/index_buffer.h>
#include <engine/api.h>

namespace tst {
namespace engine {
	namespace resources {

		class index_buffer {
        public:
            index_buffer();
            ~index_buffer();
        private:
            api::index_buffer<std::uint16_t> m_nativeIndexBuffer;
		};

	}
}
}
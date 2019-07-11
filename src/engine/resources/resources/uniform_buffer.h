// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine/api.h>
#include <engine/resources/uniform_buffer.h>

namespace tst {
namespace engine {
    namespace resources {

        class uniform_buffer {
        public:
            uniform_buffer(api::uniform_buffer&& nativeBuffer);
            ~uniform_buffer();

        private:
            api::uniform_buffer m_nativeUniformBuffer;
        };

    } // namespace resources
} // namespace engine
} // namespace tst
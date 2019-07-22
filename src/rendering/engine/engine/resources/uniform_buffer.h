// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-impl/api.h>
#include <engine-impl/resources/uniform_buffer.h>

namespace tst {
namespace engine {
    namespace resources {

        class uniform_buffer : public api::uniform_buffer {
        public:
            uniform_buffer(api::uniform_buffer&& bufferImpl);
            ~uniform_buffer();

        private:
        };

    } // namespace resources
} // namespace engine
} // namespace tst
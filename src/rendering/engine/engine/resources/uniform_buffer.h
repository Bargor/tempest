// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-impl/api.h>
#include <engine-impl/resources/uniform_buffer.h>

namespace tst {
namespace engine {
    namespace resources {

        class uniform_buffer : public api::uniform_buffer {
            using super = api::uniform_buffer;

        public:
            uniform_buffer(api::uniform_buffer&& bufferImpl);
            ~uniform_buffer();

            uniform_buffer(uniform_buffer&& other) noexcept;

        private:
        };

        static_assert(!std::is_polymorphic_v<uniform_buffer>);
        static_assert(sizeof(uniform_buffer) == sizeof(api::uniform_buffer));

    } // namespace resources
} // namespace engine
} // namespace tst
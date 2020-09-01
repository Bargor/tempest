// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-impl/api.h>
#include <engine-impl/resources/index_buffer.h>

namespace tst {
namespace engine {

    namespace resources {

        class index_buffer : private api::index_buffer {
            using super = api::index_buffer;

        public:
            index_buffer(api::index_buffer&& bufferImpl);
            ~index_buffer();

            index_buffer(index_buffer&& other) noexcept;

        public:
            TST_INLINE const api::index_buffer& to_super() const noexcept {
                return *this;
            }

        private:
        };

        static_assert(sizeof(index_buffer) == sizeof(api::index_buffer));
        static_assert(!std::is_polymorphic_v<index_buffer>);

    } // namespace resources
} // namespace engine
} // namespace tst

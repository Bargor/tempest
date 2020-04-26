// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-impl/api.h>
#include <engine-impl/resources/vertex_buffer.h>
#include <engine/resources/vertex_format.h>

namespace tst {
namespace engine {
    namespace resources {

        class vertex_buffer : private api::vertex_buffer {
            using super = api::vertex_buffer;

        public:
            vertex_buffer(api::vertex_buffer&& bufferImpl);
            ~vertex_buffer();

            vertex_buffer(vertex_buffer&& other) noexcept;

        public:
            const api::vertex_buffer& to_super() const noexcept {
                return *this;
            }

        private:
        };

        static_assert(!std::is_polymorphic_v<vertex_buffer>);
        static_assert(sizeof(vertex_buffer) == sizeof(api::vertex_buffer));
    } // namespace resources
} // namespace engine
} // namespace tst
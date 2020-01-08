// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-impl/api.h>
#include <engine-impl/resources/index_buffer.h>

namespace tst {
namespace engine {

    namespace resources {

        class index_buffer : public api::index_buffer<std::uint16_t> {
            template<typename T>
            using super = api::index_buffer<T>;

        public:
            index_buffer(api::index_buffer<std::uint16_t>&& bufferImpl);
            ~index_buffer();

            index_buffer(index_buffer&& other) noexcept;

        private:
        };

        static_assert(sizeof(index_buffer) == sizeof(api::index_buffer<std::uint16_t>));
        static_assert(!std::is_polymorphic_v<index_buffer>);

    } // namespace resources
} // namespace engine
} // namespace tst

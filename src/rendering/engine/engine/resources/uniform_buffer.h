// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-impl/api.h>
#include <engine-impl/resources/uniform_buffer.h>

namespace tst {
namespace engine {
    namespace resources {

        class uniform_buffer : private api::uniform_buffer {
            using super = api::uniform_buffer;

        public:
            using bind_point = base::resource_bind_point;

            uniform_buffer(api::uniform_buffer&& bufferImpl);
            ~uniform_buffer();

            uniform_buffer(uniform_buffer&& other) noexcept;

        public:
            const api::uniform_buffer& to_super() const noexcept {
                return *this;
            }

            template<typename StorageType>
            void update_buffer(const StorageType& data);

        private:
        };

        template<typename StorageType>
        void uniform_buffer::update_buffer(const StorageType& data) {
            super::update_buffer(&data, sizeof(data));
        }

        static_assert(!std::is_polymorphic_v<uniform_buffer>);
        static_assert(sizeof(uniform_buffer) == sizeof(api::uniform_buffer));

    } // namespace resources
} // namespace engine
} // namespace tst
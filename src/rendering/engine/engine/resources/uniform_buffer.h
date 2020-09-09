// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-impl/api.h>
#include <engine-impl/resources/uniform_buffer.h>

#include <concepts.h>

#if TST_COMPILER & TST_COMPILER_VC
#pragma warning(push)
#pragma warning(disable : 4324)
#endif

namespace tst {
namespace engine {
    namespace resources {

        struct alignas(256) uniform_storage {
            uniform_storage() = default;
        };

        template<typename T>
        concept UniformStorageType = std::derived_from<T, uniform_storage>;

        class uniform_buffer : private api::uniform_buffer {
            using super = api::uniform_buffer;

        public:

            uniform_buffer(const creation_info& info, std::uint32_t binding, std::size_t storageSize);
            ~uniform_buffer() = default;

            uniform_buffer(uniform_buffer&& other) noexcept;

        public:
            const api::uniform_buffer& to_super() const noexcept {
                return *this;
            }

            template<UniformStorageType T>
            void update_buffer(const T& data);

        private:
        };

        template<UniformStorageType T>
        void uniform_buffer::update_buffer(const T& data) {
            super::update_buffer(&data, sizeof(data));
        }

        static_assert(!std::is_polymorphic_v<uniform_buffer>);
        static_assert(sizeof(uniform_buffer) == sizeof(api::uniform_buffer));

    } // namespace resources
} // namespace engine
} // namespace tst

#if TST_COMPILER & TST_COMPILER_VC
#pragma warning(pop)
#endif
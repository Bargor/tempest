// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "buffer.h"
#include "settings.h"

#include <glm.h>
#include <vector>

#if TST_COMPILER & TST_COMPILER_VC
#pragma warning(push)
#pragma warning(disable : 4324)
#endif

namespace tst {
namespace engine {
    namespace vulkan {

        using descriptor_set = std::array<vk::DescriptorSet, settings::m_inFlightFrames>;

        struct alignas(256) uniform_storage {
            uniform_storage() = default;
        };

        class uniform_buffer : public buffer {
            using super = buffer;

        public:
            struct creation_info {
                const buffer::creation_info bufferCreationInfo;
                const descriptor_set& descriptorSets;
                const std::uint32_t& resourceIndex;
            };

        public:
            uniform_buffer(const creation_info& info, std::uint32_t binding, std::size_t storageSize);

            uniform_buffer(uniform_buffer&& other) noexcept;
            uniform_buffer(const uniform_buffer& other) = delete;

            ~uniform_buffer() = default;

            template<typename T>
            void update_buffer(const T& data);
            void update_buffer(const void* data, const std::size_t dataSize);
            vk::DescriptorSet get_descriptor_set() const noexcept;

        private:
            const std::uint32_t& m_resourceIndex;
            const std::array<vk::DescriptorSet, settings::m_inFlightFrames>& m_descriptorSets;
            std::uint32_t m_binding;
        };

        template<typename T>
        void uniform_buffer::update_buffer(const T& data) {
            update_buffer(&data, sizeof(data));
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst

#if TST_COMPILER & TST_COMPILER_VC
#pragma warning(pop)
#endif

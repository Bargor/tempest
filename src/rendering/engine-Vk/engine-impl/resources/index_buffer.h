// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "../vulkan_exception.h"
#include "buffer.h"

#include <variant>
#include <vector>

namespace tst {
namespace engine {
    namespace vulkan {

        class resource_factory;

        class index_buffer : public buffer {
            using super = buffer;

        public:
            template<typename IndexType>
            index_buffer(const buffer::creation_info& info, std::vector<IndexType>&& indices);
            ~index_buffer() = default;

            index_buffer(index_buffer&& other) noexcept;

            std::uint32_t get_index_count() const noexcept;
            vk::IndexType get_index_type() const noexcept;

        private:
            void copy_to_gpu(const buffer::creation_info& info);

        private:
            vk::IndexType m_format;
            std::variant<std::vector<std::uint16_t>, std::vector<std::uint32_t>> m_indices;
        };

        TST_INLINE std::uint32_t index_buffer::get_index_count() const noexcept {
            return std::visit([](auto&& arg) { return static_cast<std::uint32_t>(arg.size()); }, m_indices);
        }

        TST_INLINE vk::IndexType index_buffer::get_index_type() const noexcept {
            return m_format;
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst

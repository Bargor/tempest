// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "buffer.h"

#include <core.h>
#include <cstdint>
#include <variant>
#include <vector>

namespace tst {
namespace engine {
    namespace opengl {
        class index_buffer : public buffer {
        public:
            template<typename IndexType>
            index_buffer(std::vector<IndexType>&& indices);
            ~index_buffer();

            index_buffer(index_buffer&& other) noexcept;

            std::uint32_t get_index_count() const noexcept;

        private:
            std::variant<std::vector<std::uint16_t>, std::vector<std::uint32_t>> m_indices;
        };

        template<typename IndexType>
        index_buffer::index_buffer(std::vector<IndexType>&& indices)
            : buffer(indices.size() * sizeof(IndexType), indices.data(), GL_STATIC_DRAW), m_indices(std::move(indices)) {
        }

        TST_INLINE std::uint32_t index_buffer::get_index_count() const noexcept {
            return std::visit([](auto&& arg) { return static_cast<std::uint32_t>(arg.size()); }, m_indices);
        }
    } // namespace opengl
} // namespace engine
} // namespace tst
// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "buffer.h"

#include <cstdint>
#include <vector>

namespace tst {
namespace engine {
    namespace opengl {
        template<typename IndexType>
        class index_buffer : public buffer {
        public:
            index_buffer(std::vector<IndexType>&& indices);
            ~index_buffer();

            index_buffer(index_buffer&& other) noexcept;

            std::uint32_t get_index_count() const noexcept;

        private:
            std::vector<IndexType> m_indices;
        };

        template<typename IndexType>
        index_buffer<IndexType>::index_buffer(std::vector<IndexType>&& indices)
            : buffer(indices.size() * sizeof(IndexType)), m_indices(std::move(indices)) {
        }

        template<typename IndexType>
        index_buffer<IndexType>::index_buffer(index_buffer&& other) noexcept
            : buffer(std::move(other)), m_indices(std::move(other.m_indices))  {
        }

        template<typename IndexType>
        index_buffer<IndexType>::~index_buffer() {
        }

        template<typename IndexType>
        std::uint32_t index_buffer<IndexType>::get_index_count() const noexcept {
            return static_cast<std::uint32_t>(m_indices.size());
        }
    } // namespace opengl
} // namespace engine
} // namespace tst
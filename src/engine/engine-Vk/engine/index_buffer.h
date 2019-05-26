// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "buffer.h"
#include "vertex_format.h"

#include <vector>

namespace tst {
namespace engine {
    namespace vulkan {

        class index_buffer : public buffer {
        public:

        public:
            index_buffer(device& device,
                         vk::CommandPool& cmdPool,
                         vk::IndexType format,
                         std::vector<std::uint16_t>&& indices);
            ~index_buffer();

            std::uint32_t get_index_count() const;

        private:
        private:
            vk::IndexType m_format;
            std::vector<std::uint16_t> m_indices;
        };

        inline std::uint32_t index_buffer::get_index_count() const {
            return static_cast<std::uint32_t>(m_indices.size());
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
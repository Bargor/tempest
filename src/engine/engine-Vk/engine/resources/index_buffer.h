// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "buffer.h"
#include "vertex_format.h"

#include <vector>

namespace tst {
namespace engine {
    namespace vulkan {

		template<typename IndexType>
        class index_buffer : public buffer {
        public:

        public:
            index_buffer(vk::Device& logicalDevice,
				         vk::PhysicalDevice& physicalDevice,
				         vk::Queue& queueHandle,
                         vk::CommandPool& cmdPool,
                         vk::IndexType format, std::vector<IndexType>&& indices);
            ~index_buffer();

            std::uint32_t get_index_count() const;

        private:
            vk::IndexType m_format;
            std::vector<IndexType> m_indices;
        };

		template<typename IndexType>
        inline std::uint32_t index_buffer<IndexType>::get_index_count() const {
            return static_cast<std::uint32_t>(m_indices.size());
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
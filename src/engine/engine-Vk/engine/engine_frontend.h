// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "device.h"
#include "resources/index_buffer.h"

#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {

        class engine_frontend {
        public:
            engine_frontend(device& device);

            template<typename IndexType>
            index_buffer<IndexType> create_index_buffer(std::vector<IndexType>&& indices) const;

        private:
            device& m_device;
            vk::CommandPool m_commandPool;
        };

        template<typename IndexType>
        index_buffer<IndexType> engine_frontend::create_index_buffer(std::vector<IndexType>&& indices) const {
            return m_device.create_index_buffer(indices);
        }

    } // namespace vulkan

} // namespace engine
} // namespace tst
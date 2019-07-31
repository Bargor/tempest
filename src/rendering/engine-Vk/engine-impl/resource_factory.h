// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "device.h"
#include "resources/index_buffer.h"
#include "resources/uniform_buffer.h"
#include "resources/vertex_buffer.h"
#include "resources/vertex_format.h"

namespace tst {
namespace engine {

    namespace vulkan {

        class device;

        class resource_factory {
        public:
            resource_factory(device& device);
            ~resource_factory();

        public:
            template<typename IndexType>
            index_buffer<IndexType> create_index_buffer(std::vector<std::uint16_t>&& indices);
            vertex_buffer create_vertex_buffer(const vertex_format& format, std::vector<vertex>&& vertices);
            uniform_buffer create_uniform_buffer();

        private:
            device& m_device;
            vk::CommandPool m_commandPool;
        };

        template<typename IndexType>
        index_buffer<IndexType> resource_factory::create_index_buffer(std::vector<std::uint16_t>&& indices) {
            return m_device.create_index_buffer(std::move(indices), m_commandPool);
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
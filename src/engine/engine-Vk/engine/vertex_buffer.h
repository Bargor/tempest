// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "vertex_format.h"

#include <vector>

namespace tst {
namespace engine {
    namespace vulkan {

        class vertex_buffer {
        public:
            vertex_buffer(const vk::Device& device,
                          const vk::PhysicalDevice& physicalDevice,
                          const vertex_format& format,
                          std::vector<vertex>&& vertices);
            ~vertex_buffer();

			vk::Buffer get_handle() const;

        private:
            std::uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) const;
        private:
            const vk::Device& m_device;
            const vk::PhysicalDevice& m_physicalDevice;
            vertex_format m_format;
            std::vector<vertex> m_vertices;
            vk::Buffer m_buffer;
            vk::DeviceMemory m_bufferMemory;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
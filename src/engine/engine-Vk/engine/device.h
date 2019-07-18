// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "queue_indices.h"
#include "resources/index_buffer.h"
#include "resources/uniform_buffer.h"
#include "resources/vertex_buffer.h"

#include <GLFW/glfw3.h>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {

        class device {
            friend class rendering_engine;
            friend class engine_frontend;
            friend class swap_chain;
            friend class shader_compiler;

        public:
            device(GLFWwindow* window, const std::vector<const char*>& requiredExtensions);
            ~device();

            vk::CommandPool& create_command_pool();

            template<typename IndexType>
            index_buffer<IndexType> create_index_buffer(std::vector<IndexType>&& indices, vk::CommandPool& cmdPool) const;
            vertex_buffer create_vertex_buffer(const vertex_format& format,
                                               std::vector<vertex>&& vertices,
                                               vk::CommandPool& cmdPool) const;
            uniform_buffer create_uniform_buffer(const vk::CommandPool& cmdPool) const;

        private:
            vk::SurfaceKHR m_windowSurface;
            vk::PhysicalDevice m_physicalDevice;
            queue_family_indices m_queueIndices;
            vk::Device m_logicalDevice;
            vk::Queue m_graphicsQueueHandle;
            vk::Queue m_computeQueueHandle;
            vk::Queue m_presentationQueueHandle;
            vk::Queue m_transferQueueHandle;

            std::vector<vk::CommandPool> m_commandPools;
        };

        template<typename IndexType>
        index_buffer<IndexType> device::create_index_buffer(std::vector<IndexType>&& indices,
                                                            vk::CommandPool& cmdPool) const {
            return index_buffer<std::uint16_t>(m_logicalDevice,
                                               m_physicalDevice,
                                               m_graphicsQueueHandle,
                                               cmdPool,
                                               vk::IndexType::eUint16,
                                               std::move(indices));
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst

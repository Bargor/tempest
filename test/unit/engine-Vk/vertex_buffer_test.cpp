// This file is part of Tempest-engine-VK project
// Author: Karol Kontny

#include "test_environment.h"

#include <engine-impl/device.h>
#include <engine-impl/gpu_info.h>
#include <engine-impl/instance.h>
#include <engine-impl/physical_device.h>
#include <engine-impl/resources/index_buffer.h>
#include <gtest/gtest.h>

namespace tst {
namespace engine {
    namespace vulkan {

        class VertexBufferFixture : public ::testing::Test {
        public:
            VertexBufferFixture()
                : m_physicalDevice(physical_device::select_physical_device(g_surface, {}, get_required_features()))
                , m_logicalDevice(m_physicalDevice->create_logical_device(
                      instance::get_validation_layers(), {}, get_required_features()))
                , m_queue(m_logicalDevice.getQueue(m_physicalDevice->get_transfer_index(), 0))
                , m_cmdPool(m_logicalDevice.createCommandPool(
                      vk::CommandPoolCreateInfo(vk::CommandPoolCreateFlags(), m_physicalDevice->get_graphics_index()))) {
            }

            ~VertexBufferFixture() {
                m_logicalDevice.destroyCommandPool(m_cmdPool);
                m_logicalDevice.destroy();
            }

            void SetUp() {
            }

            void TearDown() {
            }

            ptr<physical_device> m_physicalDevice;
            vk::Device m_logicalDevice;
            vk::Queue m_queue;
            vk::CommandPool m_cmdPool;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
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

        class IndexBufferFixture : public ::testing::Test {
        public:
            IndexBufferFixture()
                : m_physicalDevice(physical_device::select_physical_device(
                      g_surface, {VK_KHR_SWAPCHAIN_EXTENSION_NAME}, get_required_features()))
                , m_logicalDevice(m_physicalDevice->create_logical_device(
                      instance::get_validation_layers(), {VK_KHR_SWAPCHAIN_EXTENSION_NAME}, get_required_features()))
                , m_queue(m_logicalDevice.getQueue(m_physicalDevice->get_transfer_index(), 0))
                , m_cmdPool(m_logicalDevice.createCommandPool(
                      vk::CommandPoolCreateInfo(vk::CommandPoolCreateFlags(), m_physicalDevice->get_graphics_index()))) {
            }

            ~IndexBufferFixture() {
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

        TEST_F(IndexBufferFixture, IndexBuffer16Construction) {
            index_buffer buffer({m_logicalDevice, m_queue, m_cmdPool, m_physicalDevice->get_memory_properties()},std::vector<std::uint16_t>{
            1, 2, 3});

            EXPECT_EQ(buffer.get_index_count(), 3U);
            EXPECT_EQ(buffer.get_index_type(), vk::IndexType::eUint16);
        }

        TEST_F(IndexBufferFixture, IndexBuffer32Construction) {
            index_buffer buffer({m_logicalDevice, m_queue, m_cmdPool, m_physicalDevice->get_memory_properties()},
                         std::vector<std::uint32_t>{1, 2, 3});
            EXPECT_EQ(buffer.get_index_count(), 3U);
            EXPECT_EQ(buffer.get_index_type(), vk::IndexType::eUint32);
        }

        TEST_F(IndexBufferFixture, IndexBufferMoveConstruction) {
            index_buffer buffer1({m_logicalDevice, m_queue, m_cmdPool, m_physicalDevice->get_memory_properties()},
                                 std::vector<std::uint16_t>{1, 2, 3});

            index_buffer buffer2 = std::move(buffer1);
        }

        TEST_F(IndexBufferFixture, IndexBufferMoveAssigment) {
            index_buffer buffer1({m_logicalDevice, m_queue, m_cmdPool, m_physicalDevice->get_memory_properties()},
                                 std::vector<std::uint32_t>{1, 2, 3, 4});

            const auto handle = buffer1.get_handle();

            index_buffer buffer2({m_logicalDevice, m_queue, m_cmdPool, m_physicalDevice->get_memory_properties()},
                                 std::vector<std::uint16_t>{1, 2, 3});

            buffer2 = std::move(buffer1);

            EXPECT_EQ(handle, buffer2.get_handle());
            EXPECT_EQ(buffer1.get_handle(), vk::Buffer());
            EXPECT_EQ(buffer2.get_index_count(), 4U);
            EXPECT_EQ(buffer2.get_index_type(), vk::IndexType::eUint32);
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
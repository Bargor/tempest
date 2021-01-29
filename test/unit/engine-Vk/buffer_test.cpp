// This file is part of Tempest-engine-VK project
// Author: Karol Kontny

#include "test_environment.h"

#include <engine-impl/device.h>
#include <engine-impl/gpu_info.h>
#include <engine-impl/instance.h>
#include <engine-impl/physical_device.h>
#include <engine-impl/resources/buffer.h>
#include <gtest/gtest.h>

namespace tst {
namespace engine {
    namespace vulkan {

        class BufferFixture : public ::testing::Test {
        public:
            BufferFixture()
                : m_physicalDevice(physical_device::select_physical_device(nullptr, {}, get_required_features()))
                , m_logicalDevice(m_physicalDevice->create_logical_device(
                      instance::get_validation_layers(), {}, get_required_features()))
                , m_queue(m_logicalDevice.getQueue(m_physicalDevice->get_transfer_index(), 0))
                , m_cmdPool(m_logicalDevice.createCommandPool(
                      vk::CommandPoolCreateInfo(vk::CommandPoolCreateFlags(), m_physicalDevice->get_graphics_index()))) {
            }
            ~BufferFixture() {
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

        TEST_F(BufferFixture, BufferConstruction) {
            buffer({m_logicalDevice, m_queue, m_cmdPool, m_physicalDevice->get_memory_properties()},
                   128 * settings::m_inFlightFrames,
                   vk::BufferUsageFlagBits::eUniformBuffer,
                   vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible);
        }

        TEST_F(BufferFixture, BufferMoveConstruction) {
            buffer buffer1({m_logicalDevice, m_queue, m_cmdPool, m_physicalDevice->get_memory_properties()},
                   128 * settings::m_inFlightFrames,
                   vk::BufferUsageFlagBits::eUniformBuffer,
                   vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible);

            buffer buffer2 = std::move(buffer1);
        }

        TEST_F(BufferFixture, BufferMoveAssigment) {
            buffer buffer1({m_logicalDevice, m_queue, m_cmdPool, m_physicalDevice->get_memory_properties()},
                           128 * settings::m_inFlightFrames,
                           vk::BufferUsageFlagBits::eUniformBuffer,
                           vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible);

            const auto handle = buffer1.get_handle();

            buffer buffer2({m_logicalDevice, m_queue, m_cmdPool, m_physicalDevice->get_memory_properties()},
                           128 * settings::m_inFlightFrames,
                           vk::BufferUsageFlagBits::eUniformBuffer,
                           vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible);

            buffer2 = std::move(buffer1);

            EXPECT_EQ(handle, buffer2.get_handle());
            EXPECT_EQ(buffer1.get_handle(), vk::Buffer());
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
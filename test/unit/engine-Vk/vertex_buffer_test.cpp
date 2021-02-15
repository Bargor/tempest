// This file is part of Tempest-engine-VK project
// Author: Karol Kontny

#include "test_environment.h"

#include <engine-impl/device.h>
#include <engine-impl/gpu_info.h>
#include <engine-impl/instance.h>
#include <engine-impl/physical_device.h>
#include <engine-impl/resources/vertex_buffer.h>
#include <gtest/gtest.h>

namespace tst {
namespace engine {
    namespace vulkan {

        class VertexBufferFixture : public ::testing::Test {
        public:
            VertexBufferFixture()
                : m_physicalDevice(physical_device::select_physical_device(nullptr, {}, get_required_features()))
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

        TEST_F(VertexBufferFixture, VertexBufferConstruction) {
            vertex_format format(vertex_format::primitive_topology::triangle_list);
            vertex_buffer buffer({m_logicalDevice, m_queue, m_cmdPool, m_physicalDevice->get_memory_properties()},
                                 format,
                                 std::vector<vertex>{{{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                                                     {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
                                                     {{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}});
            EXPECT_EQ(buffer.get_vertex_count(), 3U);
        }

        TEST_F(VertexBufferFixture, VertexBufferMoveConstruction) {
            vertex_format format(vertex_format::primitive_topology::triangle_list);
            vertex_buffer buffer1({m_logicalDevice, m_queue, m_cmdPool, m_physicalDevice->get_memory_properties()},
                                 format,
                                 std::vector<vertex>{{{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                                                     {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
                                                     {{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}});

            vertex_buffer buffer2 = std::move(buffer1);
        }

        TEST_F(VertexBufferFixture, VertexBufferMoveAssigment) {
            vertex_format format(vertex_format::primitive_topology::triangle_list);
            vertex_buffer buffer1({m_logicalDevice, m_queue, m_cmdPool, m_physicalDevice->get_memory_properties()},
                                  format,
                                  std::vector<vertex>{{{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                                                      {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
                                                      {{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}});

            vertex_buffer buffer2({m_logicalDevice, m_queue, m_cmdPool, m_physicalDevice->get_memory_properties()},
                                  format,
                                  std::vector<vertex>{{{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                                                      {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
                                                      {{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
                                                      {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                                                      {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
                                                      {{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}});



            const auto handle = buffer1.get_handle();

            buffer2 = std::move(buffer1);

            EXPECT_EQ(handle, buffer2.get_handle());
            EXPECT_EQ(buffer1.get_handle(), vk::Buffer());
            EXPECT_EQ(buffer2.get_vertex_count(), 3U);
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
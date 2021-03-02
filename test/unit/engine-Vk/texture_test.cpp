// This file is part of Tempest-engine-VK project
// Author: Karol Kontny

#include "test_environment.h"

#include <engine-impl/device.h>
#include <engine-impl/gpu_info.h>
#include <engine-impl/instance.h>
#include <engine-impl/physical_device.h>
#include <engine-impl/resource_cache.h>
#include <engine-impl/resources/index_buffer.h>
#include <engine-impl/resources/texture.h>
#include <gtest/gtest.h>

namespace tst {
namespace engine {
    namespace vulkan {
        class TextureFixture : public ::testing::Test {
        public:
            TextureFixture()
                : m_physicalDevice(physical_device::select_physical_device(nullptr, {}, get_required_features()))
                , m_logicalDevice(m_physicalDevice->create_logical_device(
                      instance::get_validation_layers(), {}, get_required_features()))
                , m_queue(m_logicalDevice.getQueue(m_physicalDevice->get_transfer_index(), 0))
                , m_cmdPool(m_logicalDevice.createCommandPool(
                      vk::CommandPoolCreateInfo(vk::CommandPoolCreateFlags(), m_physicalDevice->get_graphics_index())))
                , m_resourceIndex(0)
                , m_resourceCache(m_logicalDevice) {
            }

            ~TextureFixture() {
                m_resourceCache.destroy();
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
            std::uint32_t m_resourceIndex;
            resource_cache m_resourceCache;
        };

        TEST_F(TextureFixture, TextureConstruction) {
            std::array<unsigned char, 100 * 100 * 4> imageData = {};
            texture texture(
                {{m_logicalDevice, m_queue, m_cmdPool, m_physicalDevice->get_memory_properties()},
                 m_resourceCache,
                 vk::BufferUsageFlagBits::eTransferSrc,
                 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                 application::image_data{{100, 100},
                                         4,
                                         100 * 100 * 4,
                                         ptr<unsigned char, void (*)(void*)>(&imageData[0], [](void*) { return; })},
                 m_resourceIndex});
        }

        TEST_F(TextureFixture, TextureMoveConstruction) {
            std::array<unsigned char, 100 * 100 * 4> imageData = {};
            texture texture1(
                {{m_logicalDevice, m_queue, m_cmdPool, m_physicalDevice->get_memory_properties()},
                 m_resourceCache,
                 vk::BufferUsageFlagBits::eTransferSrc,
                 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                 application::image_data{{100, 100},
                                         4,
                                         100 * 100 * 4,
                                         ptr<unsigned char, void (*)(void*)>(&imageData[0], [](void*) { return; })},
                 m_resourceIndex});

            texture texture2(std::move(texture1));
        }

        TEST_F(TextureFixture, TextureMoveAssigment) {
            std::array<unsigned char, 100 * 100 * 4> imageData = {};
            texture texture1(
                {{m_logicalDevice, m_queue, m_cmdPool, m_physicalDevice->get_memory_properties()},
                 m_resourceCache,
                 vk::BufferUsageFlagBits::eTransferSrc,
                 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                 application::image_data{{100, 100},
                                         4,
                                         100 * 100 * 4,
                                         ptr<unsigned char, void (*)(void*)>(&imageData[0], [](void*) { return; })},
                 m_resourceIndex});

           texture texture2(
                {{m_logicalDevice, m_queue, m_cmdPool, m_physicalDevice->get_memory_properties()},
                 m_resourceCache,
                 vk::BufferUsageFlagBits::eTransferSrc,
                 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                 application::image_data{{100, 100},
                                         4,
                                         100 * 100 * 4,
                                         ptr<unsigned char, void (*)(void*)>(&imageData[0], [](void*) { return; })},
                 m_resourceIndex});

           texture1 = std::move(texture1);
        }
    } // namespace vulkan
} // namespace engine
} // namespace tst

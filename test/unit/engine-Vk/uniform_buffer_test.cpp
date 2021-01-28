// This file is part of Tempest-engine-VK project
// Author: Karol Kontny

#include "test_environment.h"

#include <engine-impl/device.h>
#include <engine-impl/gpu_info.h>
#include <engine-impl/instance.h>
#include <engine-impl/physical_device.h>
#include <engine-impl/resources/uniform_buffer.h>
#include <gtest/gtest.h>

namespace tst {
namespace engine {
    namespace vulkan {

        class UniformBufferFixture : public ::testing::Test {
        public:
            UniformBufferFixture()
                : m_physicalDevice(physical_device::select_physical_device(
                      g_surface, {}, get_required_features()))
                , m_logicalDevice(m_physicalDevice->create_logical_device(
                      instance::get_validation_layers(), {}, get_required_features()))
                , m_queue(m_logicalDevice.getQueue(m_physicalDevice->get_transfer_index(), 0))
                , m_cmdPool(m_logicalDevice.createCommandPool(
                      vk::CommandPoolCreateInfo(vk::CommandPoolCreateFlags(), m_physicalDevice->get_graphics_index())))
                , m_descPool(create_descriptor_pool(30, 25, 5))
                , m_resourceIndex(0) {
            }

            ~UniformBufferFixture() {
                m_logicalDevice.destroyCommandPool(m_cmdPool);
                m_logicalDevice.destroyDescriptorPool(m_descPool);
                m_logicalDevice.destroy();
            }

            void SetUp() {
                m_layout = create_layout();
            }

            void TearDown() {
                m_logicalDevice.destroyDescriptorSetLayout(m_layout);
            }

            vk::DescriptorPool create_descriptor_pool(std::uint32_t maxSets,
                                                      std::uint32_t uniformDescriptorCount,
                                                      std::uint32_t samplerDescriptorCount) {
                vk::DescriptorPoolSize uniformPoolSize(vk::DescriptorType::eUniformBuffer, uniformDescriptorCount);
                vk::DescriptorPoolSize samplerPoolSize(vk::DescriptorType::eCombinedImageSampler, samplerDescriptorCount);

                vk::DescriptorPoolCreateInfo poolCreateInfo(
                    vk::DescriptorPoolCreateFlags(),
                    maxSets,
                    2,
                    std::array<vk::DescriptorPoolSize, 2>{uniformPoolSize, samplerPoolSize}.data());

                return m_logicalDevice.createDescriptorPool(poolCreateInfo);
            }

            descriptor_set create_descriptor_set(vk::DescriptorSetLayout layout) {
                std::vector<vk::DescriptorSetLayout> layouts(settings::m_inFlightFrames, layout);
                const vk::DescriptorSetAllocateInfo allocInfo(m_descPool, settings::m_inFlightFrames, layouts.data());

                const auto sets = m_logicalDevice.allocateDescriptorSets(allocInfo);

                descriptor_set descSets;
                std::copy_n(sets.begin(), settings::m_inFlightFrames, descSets.begin());
                return descSets;
            }

            vk::DescriptorSetLayout create_layout() {
                std::vector<vk::DescriptorSetLayoutBinding> bindings;

                bindings.emplace_back(0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eAll, nullptr);

                vk::DescriptorSetLayoutCreateInfo setLayoutInfo(
                    vk::DescriptorSetLayoutCreateFlags(), static_cast<std::uint32_t>(bindings.size()), bindings.data());

                return m_logicalDevice.createDescriptorSetLayout(setLayoutInfo, nullptr);
            }

            ptr<physical_device> m_physicalDevice;
            vk::Device m_logicalDevice;
            vk::Queue m_queue;
            vk::CommandPool m_cmdPool;
            vk::DescriptorPool m_descPool;
            std::uint32_t m_resourceIndex;

            vk::DescriptorSetLayout m_layout;
        };

        TEST_F(UniformBufferFixture, UniformBufferConstruction) {
            uniform_buffer({{m_logicalDevice, m_queue, m_cmdPool, m_physicalDevice->get_memory_properties()},
                            create_descriptor_set(m_layout),
                            m_resourceIndex},
                           0,
                           sizeof(base::uniform_storage));
        }

        TEST_F(UniformBufferFixture, UniformBufferMoveConstruction) {
            uniform_buffer buffer1({{m_logicalDevice, m_queue, m_cmdPool, m_physicalDevice->get_memory_properties()},
                                    create_descriptor_set(m_layout),
                                    m_resourceIndex},
                                   0,
                                   sizeof(base::uniform_storage));

            uniform_buffer buffer2 = std::move(buffer1);
        }

        TEST_F(UniformBufferFixture, UniformBufferMoveAssigment) {
            uniform_buffer buffer1({{m_logicalDevice, m_queue, m_cmdPool, m_physicalDevice->get_memory_properties()},
                                    create_descriptor_set(m_layout),
                                    m_resourceIndex},
                                   0,
                                   sizeof(base::uniform_storage));

            const auto handle = buffer1.get_handle();

            uniform_buffer buffer2({{m_logicalDevice, m_queue, m_cmdPool, m_physicalDevice->get_memory_properties()},
                                    create_descriptor_set(m_layout),
                                    m_resourceIndex},
                                   0,
                                   sizeof(base::uniform_storage));

            const auto descriptorSet1 = buffer1.get_descriptor_set();

            buffer2 = std::move(buffer1);

            EXPECT_EQ(handle, buffer2.get_handle());
            EXPECT_EQ(buffer1.get_handle(), vk::Buffer());
            EXPECT_EQ(buffer2.get_descriptor_set(), descriptorSet1);
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
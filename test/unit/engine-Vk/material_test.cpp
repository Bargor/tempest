// This file is part of Tempest-engine-VK project
// Author: Karol Kontny

#include "test_environment.h"

#include <application/data_loader.h>
#include <engine-impl/device.h>
#include <engine-impl/gpu_info.h>
#include <engine-impl/instance.h>
#include <engine-impl/physical_device.h>
#include <engine-impl/resources/material.h>
#include <engine-impl/shader_compiler.h>
#include <gtest/gtest.h>

namespace tst {
namespace engine {
    namespace vulkan {
        class MaterialFixture : public ::testing::Test {
        public:
            MaterialFixture()
                : m_physicalDevice(physical_device::select_physical_device(nullptr, {}, get_required_features()))
                , m_logicalDevice(m_physicalDevice->create_logical_device(
                      instance::get_validation_layers(), {}, get_required_features()))
                , m_queue(m_logicalDevice.getQueue(m_physicalDevice->get_transfer_index(), 0))
                , m_cmdPool(m_logicalDevice.createCommandPool(
                      vk::CommandPoolCreateInfo(vk::CommandPoolCreateFlags(), m_physicalDevice->get_graphics_index())))
                , m_descPool(create_descriptor_pool(30, 25, 5))
                , m_resourceIndex(0)
                , m_resourceCache(m_logicalDevice)
                , m_dataLoader({g_executionDirectory, g_executionDirectory / "test_shaders"})
                , m_shaderCompiler(m_dataLoader, m_logicalDevice) {
            }

            ~MaterialFixture() {
                m_resourceCache.destroy();
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
            resource_cache m_resourceCache;
            application::data_loader m_dataLoader;
            shader_compiler m_shaderCompiler;

            vk::DescriptorSetLayout m_layout;
        };

        TEST_F(MaterialFixture, MaterialConstruction) {
            std::array<unsigned char, 100 * 100 * 4> imageData = {};

            const auto descriptorSet = create_descriptor_set(m_layout);

            std::vector<texture::creation_info> textureInfos;
            textureInfos.emplace_back(texture::creation_info{
                {m_logicalDevice, m_queue, m_cmdPool, m_physicalDevice->get_memory_properties()},
                m_resourceCache,
                vk::BufferUsageFlagBits::eTransferSrc,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                application::image_data{{100, 100},
                                        4,
                                        100 * 100 * 4,
                                        ptr<unsigned char, void (*)(void*)>(&imageData[0], [](void*) { return; })},
                m_resourceIndex});

            auto shaders = m_shaderCompiler.compile_shaders("testShaderCorrect");

            m_resourceCache.add_shaders("test_shader", std::move(shaders));

            material testMaterial(
                "test",
                "test_shader",
                material::creation_info{
                    std::move(textureInfos), descriptorSet, std::nullopt, std::nullopt, 0, 0, m_resourceIndex});
        }

        TEST_F(MaterialFixture, MaterialMoveConstruction) {
            std::array<unsigned char, 100 * 100 * 4> imageData = {};

            const auto descriptorSet = create_descriptor_set(m_layout);

            std::vector<texture::creation_info> textureInfos;
            textureInfos.emplace_back(texture::creation_info{
                {m_logicalDevice, m_queue, m_cmdPool, m_physicalDevice->get_memory_properties()},
                m_resourceCache,
                vk::BufferUsageFlagBits::eTransferSrc,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                application::image_data{{100, 100},
                                        4,
                                        100 * 100 * 4,
                                        ptr<unsigned char, void (*)(void*)>(&imageData[0], [](void*) { return; })},
                m_resourceIndex});

            auto shaders = m_shaderCompiler.compile_shaders("testShaderCorrect");

            m_resourceCache.add_shaders("test_shader", std::move(shaders));

            material testMaterial(
                "test",
                "test_shader",
                material::creation_info{
                    std::move(textureInfos), descriptorSet, std::nullopt, std::nullopt, 0, 0, m_resourceIndex});

            material testMaterial2 = std::move(testMaterial);
        }

        TEST_F(MaterialFixture, MaterialMoveAssignment) {
            std::array<unsigned char, 100 * 100 * 4> imageData = {};

            const auto descriptorSet = create_descriptor_set(m_layout);

            std::vector<texture::creation_info> textureInfos;
            textureInfos.emplace_back(texture::creation_info{
                {m_logicalDevice, m_queue, m_cmdPool, m_physicalDevice->get_memory_properties()},
                m_resourceCache,
                vk::BufferUsageFlagBits::eTransferSrc,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                application::image_data{{100, 100},
                                        4,
                                        100 * 100 * 4,
                                        ptr<unsigned char, void (*)(void*)>(&imageData[0], [](void*) { return; })},
                m_resourceIndex});

            auto shaders = m_shaderCompiler.compile_shaders("testShaderCorrect");

            m_resourceCache.add_shaders("test_shader", std::move(shaders));

            material testMaterial(
                "test",
                "test_shader",
                material::creation_info{
                    std::move(textureInfos), descriptorSet, std::nullopt, std::nullopt, 0, 0, m_resourceIndex});

            std::array<unsigned char, 100 * 100 * 4> imageData2 = {};

            std::vector<texture::creation_info> textureInfos2;
            textureInfos.emplace_back(texture::creation_info{
                {m_logicalDevice, m_queue, m_cmdPool, m_physicalDevice->get_memory_properties()},
                m_resourceCache,
                vk::BufferUsageFlagBits::eTransferSrc,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                application::image_data{{100, 100},
                                        4,
                                        100 * 100 * 4,
                                        ptr<unsigned char, void (*)(void*)>(&imageData2[0], [](void*) { return; })},
                m_resourceIndex});

            material testMaterial2(
                "test",
                "test_shader",
                material::creation_info{
                    std::move(textureInfos2), descriptorSet, std::nullopt, std::nullopt, 0, 0, m_resourceIndex});

            testMaterial2 = std::move(testMaterial);
        }
    } // namespace vulkan
} // namespace engine
} // namespace tst
// This file is part of Tempest-engine-VK project
// Author: Karol Kontny

#include "test_environment.h"

#include <application/data_loader.h>
#include <engine-impl/device.h>
#include <engine-impl/gpu_info.h>
#include <engine-impl/instance.h>
#include <engine-impl/physical_device.h>
#include <engine-impl/resources/shader.h>
#include <engine-impl/vulkan_exception.h>
#include <gtest/gtest.h>

using namespace std::literals;

namespace tst {
namespace engine {
    namespace vulkan {

        class ShaderFixture : public ::testing::Test {
        public:
            ShaderFixture()
                : m_physicalDevice(physical_device::select_physical_device(nullptr, {}, get_required_features()))
                , m_logicalDevice(m_physicalDevice->create_logical_device(
                      instance::get_validation_layers(), {}, get_required_features()))
                , m_queue(m_logicalDevice.getQueue(m_physicalDevice->get_transfer_index(), 0))
                , m_cmdPool(m_logicalDevice.createCommandPool(
                      vk::CommandPoolCreateInfo(vk::CommandPoolCreateFlags(), m_physicalDevice->get_graphics_index())))
                , m_dataLoader({g_executionDirectory}) {
            }

            ~ShaderFixture() {
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
            application::data_loader m_dataLoader;
        };

        TEST_F(ShaderFixture, ShaderConstruction) {
            try {
                const auto path = m_dataLoader.find_file("test_shaders\\testVertexShaderCorrect.spv"sv);
                shader shader(
                    m_logicalDevice, shader_type::vertex, m_dataLoader.load_shader_bytecode(path.value()), "test_shader");
            } catch (vulkan_exception&) {
                FAIL();
            }
        }

        TEST_F(ShaderFixture, ShaderMoveConstruction) {
            try {
                const auto path = m_dataLoader.find_file("test_shaders\\testVertexShaderCorrect.spv"sv);
                shader s(
                    m_logicalDevice, shader_type::vertex, m_dataLoader.load_shader_bytecode(path.value()), "test_shader");
                shader s2(std::move(s));
                EXPECT_EQ(s2.get_stage(), shader_type::vertex);
            } catch (vulkan_exception&) {
                FAIL();
            }
        }

        TEST_F(ShaderFixture, ShaderMoveAssigment) {
            try {
                const auto path = m_dataLoader.find_file("test_shaders\\testVertexShaderCorrect.spv"sv);
                shader s(
                    m_logicalDevice, shader_type::vertex, m_dataLoader.load_shader_bytecode(path.value()), "test_shader");
                shader s2(m_logicalDevice,
                          shader_type::fragment,
                          m_dataLoader.load_shader_bytecode(path.value()),
                          "test_shader_2");
                s = std::move(s2);
                EXPECT_EQ(s.get_stage(), shader_type::fragment);
            } catch (vulkan_exception&) {
                FAIL();
            }
        }

        TEST_F(ShaderFixture, VertexShaderCreationSuccess) {
            try {
                const auto path = m_dataLoader.find_file("test_shaders\\testVertexShaderCorrect.spv"sv);
                shader shader(
                    m_logicalDevice, shader_type::vertex, m_dataLoader.load_shader_bytecode(path.value()), "test_shader");
            } catch (vulkan_exception&) {
                FAIL();
            }
        }

        TEST_F(ShaderFixture, FragmentShaderCreationSuccess) {
            try {
                const auto path = m_dataLoader.find_file("test_shaders\\testFragmentShaderCorrect.spv"sv);
                shader shader(m_logicalDevice,
                              shader_type::fragment,
                              m_dataLoader.load_shader_bytecode(path.value()),
                              "test_shader");
            } catch (vulkan_exception&) {
                FAIL();
            }
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
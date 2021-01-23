// This file is part of Tempest-engine-VK project
// Author: Karol Kontny

#include "test_environment.h"

#include <engine-impl/device.h>
#include <engine-impl/gpu_info.h>
#include <engine-impl/instance.h>
#include <engine-impl/physical_device.h>
#include <engine-impl/resources/shader.h>
#include <engine-impl/vulkan_exception.h>
#include <gtest/gtest.h>

namespace tst {
namespace engine {
    namespace vulkan {

        auto testVertexShaderCorrect = "#version 450\n \
                                       void main() \
                                       { \
                                           gl_Position = vec4(0.0, 0.0, 0.0, 0.0); \
                                       }";

        auto testVertexShaderNotCorrect = "#version 450\n \
                                            void main() \
                                            { \
                                                gl_Positionasd = vec4(0.0, 0.0, 0.0, 0.0); \
                                            }";

        auto testFragmentShaderCorrect = "#version 450\n \
                                         out vec4 fragColor; \
                                         void main() { fragColor = vec4(1.0, 0.0, 0.0, 1.0); }";

        auto testFragmentShaderNotCorrect = "#version 450\n \
                                         out vec3 fragColor; \
                                         void main() { fragColor = vec4(1.0, 0.0, 0.0, 1.0); }";

        class ShaderFixture : public ::testing::Test {
        public:
            ShaderFixture()
                : m_physicalDevice(physical_device::select_physical_device(
                      g_surface, {VK_KHR_SWAPCHAIN_EXTENSION_NAME}, get_required_features()))
                , m_logicalDevice(m_physicalDevice->create_logical_device(
                      instance::get_validation_layers(), {VK_KHR_SWAPCHAIN_EXTENSION_NAME}, get_required_features()))
                , m_queue(m_logicalDevice.getQueue(m_physicalDevice->get_transfer_index(), 0))
                , m_cmdPool(m_logicalDevice.createCommandPool(
                      vk::CommandPoolCreateInfo(vk::CommandPoolCreateFlags(), m_physicalDevice->get_graphics_index()))) {
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
        };

        TEST_F(ShaderFixture, ShaderConstruction) {
            try {
                shader shader(m_logicalDevice,
                              shader_type::vertex,
                              std::vector<char>(testVertexShaderCorrect,
                                                std::next(testVertexShaderCorrect, std::strlen(testVertexShaderCorrect))),
                              "test_shader");
            } catch (vulkan_exception&) {
                FAIL();
            }
        }

        TEST_F(ShaderFixture, ShaderMoveConstruction) {
            try {
                shader s(m_logicalDevice,
                         shader_type::vertex,
                         std::vector<char>(testVertexShaderCorrect,
                                           std::next(testVertexShaderCorrect, std::strlen(testVertexShaderCorrect))),
                         "test_shader");
                shader s2(std::move(s));
                EXPECT_EQ(s2.get_stage(), shader_type::vertex);
            } catch (vulkan_exception&) {
                FAIL();
            }
        }

        TEST_F(ShaderFixture, ShaderMoveAssigment) {
            try {
                shader s(m_logicalDevice,
                         shader_type::vertex,
                         std::vector<char>(testVertexShaderCorrect,
                                           std::next(testVertexShaderCorrect, std::strlen(testVertexShaderCorrect))),
                         "test_shader");
                shader s2(m_logicalDevice,
                          shader_type::fragment,
                          std::vector<char>(testVertexShaderCorrect,
                                            std::next(testVertexShaderCorrect, std::strlen(testVertexShaderCorrect))),
                          "test_shader_2");
                s = std::move(s2);
                EXPECT_EQ(s.get_stage(), shader_type::fragment);
            } catch (vulkan_exception&) {
                FAIL();
            }
        }

        TEST_F(ShaderFixture, VertexShaderCompilationSuccess) {
            try {
                shader shader(m_logicalDevice,
                              shader_type::vertex,
                              std::vector<char>(testVertexShaderCorrect,
                                                std::next(testVertexShaderCorrect, std::strlen(testVertexShaderCorrect))),
                              "test_shader");
            } catch (vulkan_exception&) {
                FAIL();
            }
        }

        TEST_F(ShaderFixture, VertexShaderCompilationFail) {
            try {
                shader shader(
                    m_logicalDevice,
                    shader_type::vertex,
                    std::vector<char>(testVertexShaderNotCorrect,
                                      std::next(testVertexShaderNotCorrect, std::strlen(testVertexShaderNotCorrect))),
                    "test_shader");
            } catch (vulkan_exception&) {
                FAIL();
            }
        }

        TEST_F(ShaderFixture, FragmentShaderCompilationSuccess) {
            try {
                shader shader(
                    m_logicalDevice,
                    shader_type::fragment,
                    std::vector<char>(testFragmentShaderCorrect,
                                      std::next(testFragmentShaderCorrect, std::strlen(testFragmentShaderCorrect))),
                    "test_shader");
            } catch (vulkan_exception&) {
                FAIL();
            }
        }

        TEST_F(ShaderFixture, FragmentShaderCompilationFail) {
            try {
                shader shader(m_logicalDevice,
                              shader_type::fragment,
                              std::vector<char>(
                                  testFragmentShaderNotCorrect,
                                  std::next(testFragmentShaderNotCorrect, std::strlen(testFragmentShaderNotCorrect))),
                              "test_shader");
            } catch (vulkan_exception&) {
                FAIL();
            }
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
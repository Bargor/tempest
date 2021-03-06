// This file is part of Tempest-engine-VK project
// Author: Karol Kontny

#include "test_environment.h"

#include <application/data_loader.h>
#include <engine-impl/gpu_info.h>
#include <engine-impl/instance.h>
#include <engine-impl/physical_device.h>
#include <engine-impl/shader_compiler.h>
#include <gtest/gtest.h>

namespace tst {
namespace engine {
    namespace vulkan {
        class ShaderCompilerFixture : public ::testing::Test {
        public:
            ShaderCompilerFixture()
                : m_physicalDevice(physical_device::select_physical_device(nullptr, {}, get_required_features()))
                , m_logicalDevice(m_physicalDevice->create_logical_device(
                      instance::get_validation_layers(), {}, get_required_features()))
                , m_dataLoader({g_executionDirectory, g_executionDirectory / "test_shaders"}) {
            }

            ~ShaderCompilerFixture() {
                m_logicalDevice.destroy();
            }

            void SetUp() {
            }

            void TearDown() {
            }

            ptr<physical_device> m_physicalDevice;
            vk::Device m_logicalDevice;
            application::data_loader m_dataLoader;
        };

        TEST_F(ShaderCompilerFixture, ShaderCompilerConstruction) {
            shader_compiler compiler(m_dataLoader, m_logicalDevice);
        }

        TEST_F(ShaderCompilerFixture, ShaderCompilerCompilationCorrect) {
            shader_compiler compiler(m_dataLoader, m_logicalDevice);
            const auto shaderSet = compiler.compile_shaders("testShaderCorrect");

            EXPECT_EQ(shaderSet.shaders.size(), 2);
        }

        TEST_F(ShaderCompilerFixture, ShaderCompilerCompilationNotCorrect) {
            shader_compiler compiler(m_dataLoader, m_logicalDevice);
            const auto shaderSet = compiler.compile_shaders("testShaderNotCorrect");

            EXPECT_EQ(shaderSet.shaders.size(), 0);
        }
    } // namespace vulkan
} // namespace engine
} // namespace tst
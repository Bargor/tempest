// This file is part of Tempest-engine-VK project
// Author: Karol Kontny

#include "test_environment.h"

#include <engine-impl/gpu_info.h>
#include <engine-impl/physical_device.h>
#include <gtest/gtest.h>

namespace tst {
namespace engine {
    namespace vulkan {

        TEST(PhysicalDevice, PhysicalDeviceConstruction) {
            auto physicalDevice = physical_device::select_physical_device(
                g_surface, {VK_KHR_SWAPCHAIN_EXTENSION_NAME}, get_required_features());
            ASSERT_TRUE(physicalDevice);
        }

        class PhysicalDeviceFixture : public ::testing::Test {
        public:
            PhysicalDeviceFixture() {
            }

            void SetUp() {
            }

            void TearDown() {
            }
        };

        TEST_F(PhysicalDeviceFixture, PhysicalDeviceConstruction) {
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst